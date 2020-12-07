#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "tests.hpp"
#include "../CommonGatewayInterfaceRequest.hpp"

void testCGI()
{
	CommonGatewayInterfaceRequest("./test.sh").run();
}

#endif //WEBSERV_CGI_HPP
