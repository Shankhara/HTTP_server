#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "tests.hpp"
#include "../CGIExec.hpp"

void testCGI()
{
	CGIExec("./test.sh").run();
}

#endif //WEBSERV_CGI_HPP
