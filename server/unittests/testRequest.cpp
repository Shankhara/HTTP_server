#include "unittests.hpp"

#define BADREQUEST 1
#define BADMETHOD 2
#define BADVERSION 3
#define BADHEADERNAME 4
#define BADHEADER 5
#define BADBODY 6



static void assertRequest(const std::string &reqStr, const std::string &expectedMethod, const std::string &reqTarget, std::vector<Parsing::server> *servers, const std::string &testName, int expectedStatus=200)
{
	Request r(*servers);
	int status = r.doRequest(const_cast<char *>(reqStr.c_str()), reqStr.size());
	if (status != expectedStatus)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " << expectedStatus << " got " << status << std::endl;
		return ;
	}
	if (r.getMethod().compare(expectedMethod) != 0)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " << expectedMethod << " got " << r.getMethod() << std::endl;
		return ;
	}
	if (r.getReqTarget().compare(reqTarget) != 0)
	{
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > Expecting " << reqTarget << " got " << r.getReqTarget() << std::endl;
		return ;
	}
	std::cout << "\033[1;32mSuccess\033[0m > " << testName << std::endl;
}


static void testForbiddenMethod()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Parsing::server &server = vhost->at(0);
	server.locations[0].methods = std::vector<std::string>{"GET"};

	std::string str = "GET /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "GET", "/qwe", vhost, "GET OK");
	assertRequest(str, "GET", "/qwe", vhost, "POST NOK", 403);
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
	delete (vhost);
}

void correctRequestLine()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 200, "double CRLF ending requestline");

	str = "CONNECT /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "CONNECT", "/qwe", vhost, "testing CONNECT");

	str = "DELETE /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "DELETE", "/qwe", vhost, "testing DELETE");

	str = "HEAD /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "HEAD", "/qwe", vhost, "testing HEAD");

	str = "OPTIONS /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "OPTIONS", "/qwe", vhost, "testing OPTIONS");

	str = "PATCH /qwe HTTP/1.1\r\n\r\n";
	assertRequest(str, "PATCH", "/qwe", vhost, "testing PATCH");

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
	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
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

void testConstructRequest()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
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

	Request a(*vhost);
	int ret;
	std::string str = "GET /qwe HTTP/1.1\r\naccept-charsets: utf-8, iso-8859-1;q=0.5\r\n\r\n";
	ret = a.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "accept-charsets");

	Request b(*vhost);
	str = "GET /qwe HTTP/1.1\r\naccept-language: en-US,en;q=0.5\r\n\r\n";
	ret = b.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "accept-language");

	Request c(*vhost);
	str = "GET /qwe HTTP/1.1\r\nallow: GET, POST, HEAD\r\n\r\n";
	ret = c.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "allow");

	Request d(*vhost);
	str = "GET /qwe HTTP/1.1\r\nauthorization:Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n\r\n";
	ret = d.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "auth");

	Request e(*vhost);
	str = "GET /qwe HTTP/1.1\r\ncontent-language:de-DE, en-CA\r\n\r\n";
	ret = e.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "content-language");

	Request f(*vhost);
	str = "GET /qwe HTTP/1.1\r\ncontent-location:/my-first-blog-post\r\n\r\n";
	ret = f.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "content-location");

	Request g(*vhost);
	str = "GET /qwe HTTP/1.1\r\ncontent-type:/text/html; charset=UTF-8\r\n\r\n";
	ret = g.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "content-type");

	Request h(*vhost);
	str = "GET /qwe HTTP/1.1\r\ndate:Wed, 21 Oct 2015 07:28:00 GMT\r\n\r\n";
	ret = h.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "date");

	Request i(*vhost);
	str = "GET /qwe HTTP/1.1\r\nhost:developer.cdn.mozilla.net\r\n\r\n";
	ret = i.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "host");

	Request j(*vhost);
	str = "GET /qwe HTTP/1.1\r\nlast-modified:Wed, 21 Oct 2015 07:28:00 GMT\r\n\r\n";
	ret = j.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "last-modified");

	Request k(*vhost);
	str = "GET /qwe HTTP/1.1\r\nlocation:/index.html\r\n\r\n";
	ret = k.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "location");

	Request l(*vhost);
	str = "GET /qwe HTTP/1.1\r\nreferer:https://developer.mozilla.org/en-US/docs/Web/JavaScript\r\n\r\n";
	ret = l.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "referer");

	Request m(*vhost);
	str = "GET /qwe HTTP/1.1\r\nuser-agent:Mozilla/5.0\r\n\r\n";
	ret = m.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "user-agent");

	Request n(*vhost);
	str = "GET /qwe HTTP/1.1\r\ncontent-length:1\r\n\r\na";
	ret = n.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 200, "content-length");
	delete (vhost);
}

void badHeaders()
{

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request r(*vhost);
	int ret;
	std::string str = "GET /qwe HTTP/1.1\r\nReferer: 2\r\nContent-length: 3\r\n\r\n";
	ret = r.doRequest(const_cast<char *>(str.c_str()), str.size());
	assertEqual(ret, 100, "no body despite content-length header");
	//TODO: manage this error
	delete (vhost);
}

void receivedAtOnce()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();

	Request r(*vhost);
	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	assertEqual(r.parse(), 0, "correct request");

	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "wrong content-length header");

	r.request_ = "GET /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "no content-length header");

	r.request_ = "GET /qwe HTTP/1.1\r\nGET /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADHEADER, "2 correct request line");

	r.request_ = "get /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADMETHOD, "lowercase method");

	r.request_ = "GET /qwe http/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADVERSION, "lowercase version");

	r.request_ = "GET /qwe HTTP/1.1\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "1 body no headers");

	r.request_ = "GET /qwe HTTP/1.1\r\nhost: url\r\nuser-agent: hop\r\ndate: today\r\ncontent-length: 7\r\nmessage";
	assertEqual(r.parse(), BADHEADER, "no newline bet headers and body");

	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\ndate: today\r\ncontent-length: 7\r\nmessage";
	r.parse();
	assertEqual(r.queryString_parsed, true, "parsing query_string");

	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\ndate: today\r\n";
	assertEqual(r.parse(), BADHEADER, "no newline after headers with no body");

	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\nauthorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n\r\n";
	r.parse();
	assertStringEqual(r.getHeaderAuth(), "aladdin:opensesame", "correct parsing authorization");
	delete (vhost);
}


void correctChunkedBody()
{
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request a(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
	assertEqual(a.doRequest(const_cast<char*>(str.c_str()), str.size()), 200, "2 chunks with newline");
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

void testRequest()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	testConstructRequest();
	correctRequestLine();
	badRequestLine();
	correctHeaders();
	badHeaders();
	correctSequencialReceive(5);
	correctSequencialReceive(10);
	correctChunkedBody();
	badChunkedBody();
	testForbiddenMethod();
}
