#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../tests.hpp"
#include "../../CGIExec.hpp"
#include "../../RequestMock.hpp"
#include "../../fds/Client.hpp"
#include "../../fds/Listener.hpp"

bool assertHeaderStatus(const std::string &response, const std::string &status, const std::string &name)
{
	size_t found = response.find("Status: " + status);
	if (found != std::string::npos)
	{
		std::cout << "\033[1;32mSuccess\033[0m > " << name << " > assertHeaderStatus " << status << std::endl;
		return true;
	}
	std::cout << "\033[1;31mFailed\033[0m > " << name << " > assertHeaderStatus got: " << response << std::endl;
	return false;
}

void assertCGIFailed(const std::string &filename, const std::string &status, const std::string &name)
{
	Listener *listener = new Listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client *client = new Client(12, *listener);
	RequestMock  request(*client);
	CGIExec cgi;
	request.setRequestMethod("GET");
	request.setContentLength("0");
	CGIResponse *resp = cgi.run("/usr/bin/php-cgi", filename, request);
	resp->readPipe();
	assertHeaderStatus(client->getResponse(), status, name);
	close(resp->getFd());
	delete listener;
	delete client;
	delete resp;
}


void testCGI()
{
	assertCGIFailed("./notfound.php", "404", "php 404");
	assertCGIFailed("./500.php", "500", "php 500");
}

#endif //WEBSERV_CGI_HPP
