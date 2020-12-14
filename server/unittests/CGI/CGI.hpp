#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "../tests.hpp"
#include "../../CGIExec.hpp"
#include "../../Request.hpp"
#include "../../fds/Client.hpp"
#include "../../fds/Listener.hpp"

struct serversMock
{
	std::vector<std::string>		names;
	std::string						host;
	std::string						root;
	size_t							port;
	size_t							client_max_body_size;
};

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

std::string get_working_path()
{
	char temp[4096];
	return ( getcwd(temp, sizeof(temp)) ? std::string( temp ) : std::string("") );
}

void assertCGIFailed(const std::string &filename, const std::string &status, const std::string &name)
{
	Listener *listener = new Listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client *client = new Client(12, *listener);
	client->getRequest().appendRequest("GET / HTTP1.1", 13);
	CGIExec cgi;
	CGIResponse *resp = cgi.run("/usr/bin/php-cgi", get_working_path(), filename, *client);
	resp->readPipe();
	assertHeaderStatus(client->getResponse(), status, name);
	close(resp->getFd());
	delete listener;
	delete client;
	delete resp;
}

void assertCGISuccess(const std::string &filename, const std::string &name)
{
	Listener *listener = new Listener(inet_addr("127.0.0.1"), 8080, "testCGI");
	Client *client = new Client(12, *listener);
	client->getRequest().appendRequest("GET / HTTP1.1", 13);
	CGIExec cgi;
	CGIResponse *resp = cgi.run("/usr/bin/php-cgi", get_working_path(), filename, *client);
	resp->readPipe();
	assertHeaderStatus(client->getResponse(), "200", name);
	close(resp->getFd());
	delete listener;
	delete client;
	delete resp;
}


void testCGI()
{
	assertCGIFailed("/notfound.php", "404", "php 404");
	assertCGIFailed("/500.php", "500", "php 500");
	assertCGISuccess("/200.php", "php_info()");
}

#endif //WEBSERV_CGI_HPP
