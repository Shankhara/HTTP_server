#include "unittests.hpp"
#include <string.h>

static bool assertBuildWithoutException(Response *resp, const std::string &testName)
{
	try {
		resp->build();
		std::cout << "\033[1;32mSuccess\033[0m > " << testName << " > no exception as expected" << std::endl;
		return true;
	} catch (RespException &e) {
		std::cout << "\033[1;31mFail\033[0m: > " << testName << " > got unexpected exception: " << e.getStatusCode() << std::endl;
		return false;
	}
}

void testRespGet()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string str = "GET /index.html HTTP/1.1\r\n\r\n";
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespGet respGet(ra, buf, bufsize);
	if (!assertBuildWithoutException(&respGet, __FUNCTION__ ))
		return ;

	int readSize = respGet.readResponse();
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
	delete (vhost);
}

void testRespPut()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string body = "HTTP is a generic interface protocol for information systems. \
	It is designed to hide the details of how a service is ...";
	std::string str = "PUT /a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespPut respPut(ra, buf, bufsize);
	assertBuildWithoutException(&respPut, __FUNCTION__ );

	int readSize = respPut.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
	delete (vhost);
}

void testRespPost()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string body = "HTTP is a generic interface protocol for information systems. \
	It is designed to hide the details of how a service is ..."; 
	std::string str = "POST /a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespPost respPost(ra, buf, bufsize);
	assertBuildWithoutException(&respPost, __FUNCTION__ );

	int readSize = respPost.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;

	str = "POST /new_dir/a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	Request rb(*vhost);
	rb.doRequest(const_cast<char*>(str.c_str()), str.size());

	RespPost respPost_b(rb, buf, bufsize);
	assertBuildWithoutException(&respPost_b, __FUNCTION__ );

	readSize = respPost_b.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;

	str = "POST /a/b/c/d/new_dir/a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	Request rc(*vhost);
	rc.doRequest(const_cast<char*>(str.c_str()), str.size());
	RespPost respPost_c(rc, buf, bufsize);
	assertBuildWithoutException(&respPost_c, __FUNCTION__ );

	readSize = respPost_c.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;

	delete (vhost);
}

void testRespDelete()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request r(*vhost);
	std::string str = "DELETE /a.txt HTTP/1.1\r\nHost: webserv\r\n\r\n";
	r.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespDelete respDel(r, buf, bufsize);
	assertBuildWithoutException(&respDel, __FUNCTION__ );

	int readSize = respDel.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
}

void testRespTrace()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];
	memset(buf, 0, bufsize);

    std::string str = "TRACE /index.html HTTP/1.1\r\n\r\n";
    ra.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespTrace respTrace(ra, buf, bufsize);
	assertBuildWithoutException(&respTrace, __FUNCTION__ );

	respTrace.readResponse();
	std::cout << "|" << buf << "|" << std::endl;

	Request rb(*vhost);
	str = "TRACE /index.html HTTP/1.1\r\nHost: localhost:8080\r\nContent-length: 7\r\n\r\nbonjour";
	rb.doRequest(const_cast<char*>(str.c_str()), str.size());
	memset(buf, 0, bufsize);
	RespTrace respTrace_b(rb, buf, bufsize);
	assertBuildWithoutException(&respTrace_b, __FUNCTION__ );

	respTrace_b.readResponse();
	std::cout << "|" << buf << "|" << std::endl;

	Request rc(*vhost);
	str = "TRACE /index.html HTTP/1.1\r\nHost: localhost:8080\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
	rc.doRequest(const_cast<char*>(str.c_str()), str.size());
	memset(buf, 0, bufsize);
	RespTrace respTrace_c(rc, buf, bufsize);
	assertBuildWithoutException(&respTrace_c, __FUNCTION__ );
	respTrace_c.readResponse();
	std::cout << "|" << buf << "|" << std::endl;

	delete (vhost);
}


void testRespOptions()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string str = "OPTIONS /index.html HTTP/1.1\r\n\r\n";
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespOptions respOptions(ra, buf, bufsize);
	assertBuildWithoutException(&respOptions, __FUNCTION__ );

	int readSize = respOptions.readResponse();
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
	delete (vhost);
}

void testRespError()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string str = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nAuthorization: qwe\r\n\r\n";
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());

	unsigned int bufsize = 16 * 1024;
	char buf[bufsize];

	RespError respError(401, ra, buf, bufsize);
	assertBuildWithoutException(&respError, __FUNCTION__ );

	int readSize = respError.readResponse();
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
	delete (vhost);
}


void testMimeType()
{
	Mime m;
	std::string fileName = "index.html";
	std::string ret;

	ret = m.getContentType(fileName);
	assertStringEqual(ret, "text/html", "fileName: " + fileName);

	fileName = "index.html.fr";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "text/html", "fileName: " + fileName);

	fileName = "file.rar";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "application/rar", "fileName: " + fileName);

	fileName = "pres.ppt";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "application/vnd.ms-powerpoint", "fileName: " + fileName);

	fileName = "image.tiff";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "image.tiff.fr.DE.GE";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "dir/image.tiff";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "/path/dir/image.tiff";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "/path/dir/image/";
	ret = m.getContentType(fileName);
	assertStringEqual(ret, "", "fileName: " + fileName);

	fileName = "image.tiff";
	ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);
}

void testLangNegotiation()
{
    std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

    std::vector<Parsing::server> *vhost = createVirtualHosts();
    Request ra(*vhost);
    unsigned int bufsize = 16 * 1024;
    char buf[bufsize];

    std::string str = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nAccept-language: fr-CH, fr;q=0.9, en;q=0.8, de;q=0.7, *;q=0.5\r\n\r\n";
    ra.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespGet respGet(ra, buf, bufsize);
    respGet.build();
    int readSize = respGet.readResponse();
    buf[readSize] = '\0';
    std::cout << "|" << buf << "|" << std::endl;
    delete (vhost);
}

void testResponse()
{
//	testRespGet();
//	testRespPut();
//	testRespPost();
// 	testRespTrace();
//	testRespDelete();
//	testRespOptions();
//	testRespError();
//	testMimeType();
	testLangNegotiation();
}