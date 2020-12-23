#include "unittests.hpp"

void testResponse()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	Log().setLevel(logDEBUG);
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string str = "GET /index.html HTTP/1.1\r\n\r\n";
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());
	GetMethod a(ra);

	a.exec();
	std::cout << a.getResponseMsg() << std::endl;
}
