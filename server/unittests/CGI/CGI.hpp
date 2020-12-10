#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../tests.hpp"
#include "../../CGIExec.hpp"
#include "../../RequestMock.hpp"
#include "../../fds/Client.hpp"
#include "../../fds/Listener.hpp"

void testCGI()
{
	Listener *listener = new Listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client *client = new Client(12, *listener);
	RequestMock  request(*client);
	CGIExec cgi;
	request.setRequestMethod("GET");
	request.setContentLength("0");
	try {
		cgi.run("/tmp/test.sh", request);
	} catch (std::exception &e) {
		std::cout << "Exception: " << e.what() << std::endl;
	}
	Server::releaseInstance();
}

#endif //WEBSERV_CGI_HPP
