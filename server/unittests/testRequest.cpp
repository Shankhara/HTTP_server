#include "unittests.hpp"

static void assertRequest(const std::string &reqStr, const std::string &expectedMethod, \
const std::string &reqTarget, const std::vector<Parsing::server> *servers, const std::string &testName, int expectedStatus=200)
{
	Request r(*servers);
	int status = r.doRequest(const_cast<char *>(reqStr.c_str()), reqStr.size());
	if (status != expectedStatus)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " \
		<< expectedStatus << " got " << status << std::endl;
		return ;
	}
	if (r.getMethod().compare(expectedMethod) != 0)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " \
		<< expectedMethod << " got " << r.getMethod() << std::endl;
		return ;
	}
	if (r.getReqTarget().compare(reqTarget) != 0)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " \
		<< reqTarget << " got " << r.getReqTarget() << std::endl;
		return ;
	}
	std::cout << "\033[1;32mSuccess\033[0m > " << testName << std::endl;
}


static void testForbiddenMethod()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();

	// Only allowing GET with location /
	Parsing::server &server = vhost->at(0);
	server.locations[0].methods = std::vector<std::string>{"GET"};

	std::string str = "GET /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "GET Ok");
	// Head is supposed to fail
	str = "HEAD /qwe/head HTTP/1.1\r\n\r\n";
	assertRequest(str, "HEAD", "/qwe/head", vhost, "HEAD Forbidden", 405);

	// adding a more specific location without any restriction, head isnt forbidden anymore
	server.locations.push_back(Parsing::location());
	server.locations[1].name = std::string("/qwe/");
	assertRequest(str, "HEAD", "/qwe/head", vhost, "HEAD ok", 200);
	delete (vhost);
}

void badRequestLine()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 100, "not double CRLF ended");

	Request b(*vhost);
	str = "GETGET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(b.doRequest(const_cast<char*>(str.c_str()), str.size()), 501, "bad method");

	Request c(*vhost);
	str = "GET GET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(c.doRequest(const_cast<char*>(str.c_str()), str.size()), 400, "4 elements");

	Request d(*vhost);
	str = "GET /qweHTTP/1.1\r\n\r\n";
	assertEqual(d.doRequest(const_cast<char*>(str.c_str()), str.size()), 400, "1 element");

	Request e(*vhost);
	str = "GET /qwe HTTP1.1\r\n\r\n";
	assertEqual(e.doRequest(const_cast<char*>(str.c_str()), str.size()), 505, "bad version");

	Request f(*vhost);
	str = "GET NOPE HTTP/1.1\r\nHost: localhost\r\n\r\n";
	assertEqual(f.doRequest(const_cast<char*>(str.c_str()), str.size()), 403, "invalid target NOPE");

	Request g(*vhost);
	str = "CONNECT /qwe HTTP/1.1\r\nHost: localhost\r\n\r\n";
	assertEqual(g.doRequest(const_cast<char*>(str.c_str()), str.size()), 501, "method not implemented");

	delete (vhost);
}

void correctRequestLine()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 200, "double CRLF ending requestline");

	str = "DELETE /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "DELETE", "/qwe", vhost, "testing DELETE");

	str = "HEAD /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "HEAD", "/qwe", vhost, "testing HEAD");

	str = "OPTIONS /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "OPTIONS", "/qwe", vhost, "testing OPTIONS");

	str = "POST /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "POST", "/qwe", vhost, "testing POST");

	str = "PUT /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "PUT", "/qwe", vhost, "testing PUT");

	str = "TRACE /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "TRACE", "/qwe", vhost, "testing TRACE");

	delete (vhost);
}

void correctSequencialReceive(size_t len)
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" << __FUNCTION__ << "\033[0m" << std::endl;

	Request r(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nHost: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	size_t start = 0;

	while (start + len < str.size())
	{
		r.doRequest(const_cast<char*>(str.c_str()) + start, len);
		start += len;
		assertEqual(r.getStatusCode(), 100, "status code while receiving");
	}
	len = str.size() - start;
	r.doRequest(const_cast<char*>(str.c_str()) + start, len);
	assertEqual(r.getStatusCode(), 200, "status code at end");
	delete (vhost);
}

void testDoRequest()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request r(*vhost);
	int ret;

	std::string req("GET / HTTP/1.1\r\n\r\n");

	ret = r.doRequest(const_cast<char *>(req.c_str()), 16);
	assertEqual(ret, 100, "Request not over");
	ret = r.doRequest(const_cast<char *>(req.c_str()) + 16, 2);
	assertEqual(ret, 200, "Request over");
	delete (vhost);
}

void correctHeaders()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	int ret;
	std::string str = "GET /qwe HTTP/1.1\r\naccept-charsets: utf-8, iso-8859-1;q=0.5\r\n\r\n";

	Request o(*vhost);
	str = "GET /qwe HTTP/1.1\r\nDate: \t2\r\n\r\n";
	ret = o.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertStringEqual(o.getHeaderDate(), "2", "replace whitespace in header value");

	Request p(*vhost);
	str = "GET /qwe HTTP/1.1\r\nDate: \t2\r\nHost: mo\fmo \r\n\r\n";
	ret = p.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertStringEqual(p.getHeaderDate(), "2", "replace whitespace in header value");
	assertStringEqual(p.getHeaderHost(), "momo", "replace whitespace in header value");
	delete (vhost);
}

void badHeaders()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	Request a(*vhost);

	std::string str = "GET /qwe HTTP/1.1\r\nHost: 2\r\nContent-length: 3\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "no body despite content-length header", 100);

	str = "GET /qwe HTTP/1.1\r\nHost : 2\r\nDate: today\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "whitespace between header name and colon", 400);

	str = "GET /qwe HTTP/1.1\r\nHost:: 2\r\nDate: today\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "two ':'", 400);

	str = "GET /qwe HTTP/1.1\r\nHost::: 2\r\nDate: today\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "three ':'", 400);

	delete (vhost);
}

void testCGIHeaders()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	int ret;
	std::string str = "GET /qwe HTTP/1.1\r\nX-custom1: a\r\nHost: 2\r\nContent-length: 0\r\nX-custom2: b\r\nX-custom3: c\r\n\r\n";
	ret = a.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "test getter");
	std::map<std::string, std::string> tmp = a.getCGIHeaders();
//	for (std::map<std::string, std::string>::iterator it = tmp.begin(); it != tmp.end(); it++)
//		std::cout << it->first << " => " << it->second << std::endl;

}

void correctChunkedBody()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\nhost: hop\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 200, "2 chunks with newline");

	Request b(*vhost);
	std::string tester = "POST / HTTP/1.1\r\nHost: localhost:8080\r\nUser-Agent: Go-http-client/1.1\r\nTransfer-Encoding: chunked\r\nContent-Type: test/file\r\nAccept-Encoding: gzip\r\n\r\n";
	assertEqual(b.doRequest(const_cast<char*>(tester.c_str()), tester.size()), 200, "42 tester");
	delete (vhost);
}

void badChunkedBody()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n0\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 400, "bad chunk size");

	Request b(*vhost);
	str= "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n0\n\r\n";
	assertEqual(b.doRequest(const_cast<char*>(str.c_str()), str.size()), 400, "bad end");

	Request c(*vhost);
	str = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n4\r\n\r\n";
	assertEqual(c.doRequest(const_cast<char*>(str.c_str()), str.size()), 400, "bad end 2");

	delete (vhost);
}

static void testBodyWithGet()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request a(*vhost);

	std::string str = "GET /qwe HTTP/1.1\r\nHost: webserv\r\nContent-length: 1\r\n\r\nt";
	assertRequest(str, "GET", "/qwe", vhost, "testing GET with a body", 200);

	str = "GET /qwe HTTP/1.1\r\nHost: webserv\r\nContent-length: 1\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "testing GET with content-len = 1 but no body", 100);

	str = "GET /qwe HTTP/1.1\r\nHost: webserv\r\nContent-length: 0\r\n\r\nqweqwe";
	assertRequest(str, "GET", "/qwe", vhost, "testing GET with content-len = 0 with a body", 200);
	//Could work if I add more tests to parseHeadersContent for Content-len & TE

	delete (vhost);
}

static void testIncorrectContentLength()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);

	std::string req = "POST /test.bla HTTP/1.1\r\nContent-Length: 4\r\nHost: localhost\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nDATA IS NOT 4";
	assertEqual(a.doRequest(const_cast<char*>(req.c_str()), req.size()), 400, "Content-Length < bodySize");

	delete (vhost);
}

static void testMatchLocation()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	vhost->at(0).locations.push_back(Parsing::location());
	vhost->at(0).locations[1].name = std::string("/directory");
	vhost->at(0).locations[1].methods.push_back("POST");
	vhost->at(0).locations[1].root = "/tmp";
	vhost->at(0).locations.push_back(Parsing::location());
	vhost->at(0).locations[2].name = std::string("/put_test/");
	vhost->at(0).locations[2].methods.push_back("PUT");

	Request a(*vhost);
	std::string req = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(req.c_str()), req.size()), 200, "matchLocation GET /");

	Request b(*vhost);
	std::string req2 = "POST /put_test/cgi.bla HTTP/1.1\r\nHost: localhost\r\n\r\n";
	assertEqual(b.doRequest(const_cast<char*>(req2.c_str()), req2.size()), 405, "matchLocation POST forbidden");

	Request c(*vhost);
	std::string req3 = "POST /directory/cgi.bla HTTP/1.1\r\nHost: localhost\r\n\r\n";
	assertEqual(c.doRequest(const_cast<char*>(req3.c_str()), req3.size()), 200, "matchLocation POST");

	delete (vhost);
}

void testStrToHex()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	assertEqual(strHexToInt("e"), (unsigned long)14, "StrToHex() e");
	assertEqual(strHexToInt("3e8"), (unsigned long)1000, "StrToHex() 3e8");
	assertEqual(strHexToInt("3E8"), (unsigned long)1000, "StrToHex() 3E8");
}

static void testServerMatch()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhosts = createVirtualHosts();
	Parsing::server server;
	server.host = "127.0.0.1";
	server.port = 8080;
	server.names = std::vector<std::string>{"google.com"};
	server.root = "/tmp/";
	server.locations.push_back(Parsing::location());
	server.locations[0].name = std::string("/");
	server.locations[0].root = "/tmp";
	server.locations[0].methods = std::vector<std::string>{"HEAD"};
	vhosts->push_back(server);
	Request a(*vhosts);
	std::string req = "GET /directory/cgi.bla HTTP/1.1\r\nHost: idonoexist\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(req.c_str()), req.size()), 200, "matchServer fallback to default GET is authorized");
	Request b(*vhosts);
	req = "GET /directory/cgi.bla HTTP/1.1\r\nHost: google.com\r\n\r\n";
	assertEqual(b.doRequest(const_cast<char*>(req.c_str()), req.size()), 405, "matchServer google.com GET is forbidden");
	Request c(*vhosts);
	req = "GET /directory/cgi.bla HTTP/1.1\r\nHost: google.com:8080\r\n\r\n";
	assertEqual(c.doRequest(const_cast<char*>(req.c_str()), req.size()), 405, "matchServer google.com:8080 GET is forbidden");
}

static void testExplode()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<std::string> res = explode("Host: 8080", ':', 1);
	assertEqual(res.at(0), std::string("Host"), "explode title");
	assertEqual(res.at(1), std::string(" 8080"), "explode value");

}

void testRequest()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	testDoRequest();
  	correctRequestLine();
	badRequestLine();
	correctHeaders();
 	badHeaders();
	testCGIHeaders();
	correctSequencialReceive(5);
	correctSequencialReceive(10);
	correctChunkedBody();
	badChunkedBody();
	testBodyWithGet();
	testForbiddenMethod();
	testMatchLocation();
	testIncorrectContentLength();
	testStrToHex();
	testServerMatch();
	testExplode();
}
