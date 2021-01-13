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
	std::string str = "GET /test/ HTTP/1.1\r\n\r\n";
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

static void requestGenerator(std::string method, std::string reqTarget, std::string headers, std::string
body)
{
    std::vector<Parsing::server> *vhost = createVirtualHosts();
    Request a(*vhost);
    unsigned int bufsize = 16 * 1024;
    char buf[bufsize];
    Response *resp;

    std::string str = method +" "+ reqTarget +" HTTP/1.1\r\n"+ headers +"\r\n\r\n" + body;
    a.doRequest(const_cast<char*>(str.c_str()), str.size());
    if (method == "GET")
        resp = new RespGet(a, buf, bufsize);
    if (method == "POST")
        resp = new RespPost(a, buf, bufsize);
    if (method == "PUT")
        resp = new RespPut(a, buf, bufsize);
    if (method == "DELETE")
        resp = new RespDelete(a, buf, bufsize);
    if (method == "OPTIONS")
        resp = new RespOptions(a, buf, bufsize);
    if (method == "TRACE")
        resp = new RespTrace(a, buf, bufsize);
    try {
    	resp->build();
    } catch (const RespException &e) {
    	Log::get(logINFO) << "Unable to build request: " << e.getStatusCode() << std::endl;
    	return ;
    }
	size_t readSize = resp->readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;
//    if (!assertBuildWithoutException(resp, __FUNCTION__ ))
  //      return;
    delete (vhost);
}

//void testRespPut()
//{
//	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
//
//	std::string body = "HTTP is a generic interface protocol for information systems. \
//	It is designed to hide the details of how a service is ...";
//    std::string method = "PUT";
//	std::string reqTarget = "/test/a.txt";
//    std::string headers = "HTTP/1.1\r\nHost: webserv\r\nContent-length: " + ft_itoa(body.size());
//	std::string str = method + reqTarget + headers + "\r\n\r\n" + body;
//	requestGenerator("PUT", "test/a.txt",\
//	"HTTP/1.1\r\nHost: webserv\r\nContent-length: " + ft_itoa(body.size()), body);
//
//	unsigned int bufsize = 16 * 1024;
//	char buf[bufsize];
//
//	RespPut respPut(ra, buf, bufsize);
//	assertBuildWithoutException(&respPut, __FUNCTION__ );
//
//	size_t readSize = respPut.readResponse();
//	Log::get(logDEBUG) << "READ " << readSize << std::endl;
//	buf[readSize] = '\0';
//	std::cout << "|" << buf << "|" << std::endl;
//	delete (vhost);
//}

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
	std::string fileName = "index.html";
	std::string ret;

    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "text/html", "fileName: " + fileName);

	fileName = "file.rar";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "application/rar", "fileName: " + fileName);

	fileName = "pres.ppt";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "application/vnd.ms-powerpoint", "fileName: " + fileName);

	fileName = "image.tiff";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "dir/image.tiff";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "/path/dir/image.tiff";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);

	fileName = "/path/dir/image/";
    ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "application/octet-stream", "fileName: " + fileName);

	fileName = "image.tiff";
	ret = Mime::getInstance()->getContentType(fileName);
	assertStringEqual(ret, "image/tiff", "fileName: " + fileName);
}

void testNegotiateContentLang ()
{
    std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

    std::vector<Parsing::server> *vhost = createVirtualHosts();
    Request ra(*vhost);
    unsigned int bufsize = 16 * 1024;
    char buf[bufsize];

    std::string str = "POST /index.html HTTP/1.1\r\nHost: localhost:8080\r\ncontent-language: "
                      "fr-CH, FR, DE, US\r\ncontent-length: 5\r\n\r\nabcde";
    ra.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespPost respPost(ra, buf, bufsize);
    if (!assertBuildWithoutException(&respPost, __FUNCTION__ ))
        return ;
    int readSize = respPost.readResponse();
    buf[readSize] = '\0';
    std::cout << "|" << buf << "|" << std::endl;

    Request rb(*vhost);
    str = "PUT /index.html HTTP/1.1\r\nHost: localhost:8080\r\ncontent-language: "
          "fr-CH, FR, DE, US\r\ncontent-length: 5\r\n\r\nabcde";
    rb.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespPut respPut(rb, buf, bufsize);
    if (!assertBuildWithoutException(&respPut, __FUNCTION__ ))
        return ;
    readSize = respPut.readResponse();
    buf[readSize] = '\0';
    std::cout << "|" << buf << "|" << std::endl;
    delete (vhost);
}

void testNegotiateAcceptLang()
{
    std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

    std::vector<Parsing::server> *vhost = createVirtualHosts();
    Request ra(*vhost);
    unsigned int bufsize = 16 * 1024;
    char buf[bufsize];

    std::string str = "GET /index.html HTTP/1.1\r\nHost: localhost:8080\r\nAccept-language: fr-CH, "
                      "FR, de, uS\r\n\r\n";
    ra.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespGet respGet(ra, buf, bufsize);
    if (!assertBuildWithoutException(&respGet, __FUNCTION__ ))
    	return ;
    int readSize = respGet.readResponse();
    buf[readSize] = '\0';
    std::cout << "|" << buf << "|" << std::endl;
    delete (vhost);
}

void testRespDelete()
{
    std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

    std::vector<Parsing::server> *vhost = createVirtualHosts();
    Request r(*vhost);
    std::string str = "DELETE /index.html HTTP/1.1\r\nHost: webserv\r\ncontent-language:FR, US\r\n\r\n";

    unsigned int bufsize = 16 * 1024;
    char buf[bufsize];

    r.doRequest(const_cast<char*>(str.c_str()), str.size());
    RespDelete respDel(r, buf, bufsize);
    assertBuildWithoutException(&respDel, __FUNCTION__ );

    int readSize = respDel.readResponse();
    Log::get(logDEBUG) << "READ " << readSize << std::endl;
    buf[readSize] = '\0';
    std::cout << "|" << buf << "|" << std::endl;
}

void tester()
{
    requestGenerator("GET", "index.html", "host: localhost:8080", "");

}

void testResponse()
{
    tester();
//	testRespPut();
//    testRespGet();
//	testRespPost();
// 	testRespTrace();
//	testRespOptions();
//	testRespError();
//	testMimeType();
//	testNegotiateContentLang();
//    testNegotiateAcceptLang();
//    testRespDelete();
}
