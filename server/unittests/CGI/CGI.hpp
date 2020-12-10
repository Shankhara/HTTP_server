#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../tests.hpp"
#include "../../CGIExec.hpp"
#include "../../RequestMock.hpp"
#include "../../fds/Client.hpp"
#include "../../fds/Listener.hpp"

void testCGI()
{
	Listener listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client client(12, listener);
	RequestMock  request(client);
	request.setRequestMethod("GET");
	request.setContentLength("0");
	try {
		CGIExec::getInstance()->run("/tmp/test.sh", request);
	} catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
}

#endif //WEBSERV_CGI_HPP
