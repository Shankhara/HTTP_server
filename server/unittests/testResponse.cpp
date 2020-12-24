#include "unittests.hpp"

void testResponse()
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
