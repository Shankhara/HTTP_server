#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../tests.hpp"
#include "../../CGIExec.hpp"
#include "../../RequestMock.hpp"
#include "../../fds/Client.hpp"
#include "../../fds/Listener.hpp"

void assertCGIFailed(const std::string &cgiScript, const std::string &name)
{
	Listener *listener = new Listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client *client = new Client(12, *listener);
	RequestMock  request(*client);
	CGIExec cgi;
	request.setRequestMethod("GET");
	request.setContentLength("0");
	CGIResponse *resp = cgi.run(cgiScript, request);
	resp->readPipe();
	std::cerr << client->getResponse() << std::endl;
	assertStringEqual(client->getResponse(),"", name);
	delete Server::getInstance();
}


void testCGI()
{
	//assertCGIFailed("./test.sh", "test shell script");
	assertCGIFailed("/usr/bin/php-cgi", "php-cgi");
}

#endif //WEBSERV_CGI_HPP
