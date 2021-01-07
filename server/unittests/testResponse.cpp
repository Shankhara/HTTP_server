#include "unittests.hpp"
#include <string.h>

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

	int readSize = respGet.readResponse();
	std::cout << "#" << readSize << "#" << std::endl;
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

	int readSize = respPost.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;

	str = "POST /new_dir/a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	Request rb(*vhost);
	rb.doRequest(const_cast<char*>(str.c_str()), str.size());
	RespPost respPost_b(rb, buf, bufsize);

	readSize = respPost_b.readResponse();
	Log::get(logDEBUG) << "READ " << readSize << std::endl;
	buf[readSize] = '\0';
	std::cout << "|" << buf << "|" << std::endl;

	str = "POST /a/b/c/d/new_dir/a.txt HTTP/1.1\r\nHost: webserv\r\nContent-length: " \
	+ ft_itoa(body.size()) + "\r\n\r\n" + body;
	Request rc(*vhost);
	rc.doRequest(const_cast<char*>(str.c_str()), str.size());
	RespPost respPost_c(rc, buf, bufsize);

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

	respTrace.readResponse();
	std::cout << "|" << buf << "|" << std::endl;

	Request rb(*vhost);
	str = "TRACE /index.html HTTP/1.1\r\nHost: localhost:8080\r\nContent-length: 7\r\n\r\nbonjour";
	rb.doRequest(const_cast<char*>(str.c_str()), str.size());
	memset(buf, 0, bufsize);
	RespTrace respTrace_b(rb, buf, bufsize);
	respTrace_b.readResponse();
	std::cout << "|" << buf << "|" << std::endl;

	Request rc(*vhost);
	str = "TRACE /index.html HTTP/1.1\r\nHost: localhost:8080\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
	rc.doRequest(const_cast<char*>(str.c_str()), str.size());
	memset(buf, 0, bufsize);
	RespTrace respTrace_c(rc, buf, bufsize);
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

	int readSize = respOptions.readResponse();
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

void testResponse()
{
	testRespGet();
	testRespPut();
	testRespPost();
 	testRespTrace();
	testRespDelete();
	testRespOptions();
	testMimeType();
}
