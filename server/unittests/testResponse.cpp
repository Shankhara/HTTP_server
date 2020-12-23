#include "unittests.hpp"

void testResponse()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	std::vector<Parsing::server> *vhost = createVirtualHosts();
	Request ra(*vhost);
	std::string str = "GET /qwe HTTP/1.1\r\n\r\n";
	ra.doRequest(const_cast<char*>(str.c_str()), str.size());
	Response a(ra);

	a.createResponse();
	std::cout << a.getResponseMsg() << std::endl;
}
