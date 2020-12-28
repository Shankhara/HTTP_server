#include "unittests.hpp"

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
	Log().Get(logDEBUG) << "READ " << readSize;
	buf[readSize] = '\0';
	std::cout << buf << std::endl;
	delete (vhost);
}

void testMimeType()
{
	Mime m;
	std::string fileName = "index.html";
	std::string ret;

	ret = m.getContentType(fileName);
	std::cout << ret << std::endl;
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
}

void testResponse()
{
//	testRespGet();
	testMimeType();
}
