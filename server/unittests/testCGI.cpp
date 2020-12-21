#ifndef WEBSERV_CGI_HPP
#define WEBSERV_CGI_HPP

#include "unittests.hpp"

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

static std::string readAllCGIResponse(int fd)
{
	std::string output;
	char buf[256];
	int n = 0;
	bool r = false;

	while (r == false){
		while ((n = read(fd, buf, 255)) > 0)
		{
			r = true;
			output.append(buf, n);
		}
		if (n < 0)
			break;
		usleep(10000);
	}
	return output;
}

void assertCGIFailed(const std::string &filename, const std::string &status, const std::string &name)
{
	std::vector<Parsing::server> *servers = createVirtualHosts();
	Listener *listener = new Listener();
	listener->addServer(servers->at(0));
	Client *client = new Client(12, *servers);
	std::string req = "GET / HTTP1.1\r\n\r\n";
	client->getRequest().doRequest(const_cast<char *>(req.c_str()), req.size());
	CGIExec cgi;
	std::string path = get_working_path() + "/cgi";
	FileDescriptor *resp = cgi.run("/usr/bin/php-cgi", path.c_str(), filename, *client);
	std::string response = readAllCGIResponse(resp->getFd());
	assertHeaderStatus(response, status, name);
	close(resp->getFd());
	delete servers;
	delete listener;
	delete client;
	delete resp;
}



void assertCGISuccess(const std::string &filename, const std::string &name)
{
	std::vector<Parsing::server> *servers = createVirtualHosts();
	Listener *listener = new Listener();
	listener->addServer(servers->at(0));
	Client *client = new Client(12, *servers);
	std::string req = "POST /test.bla HTTP/1.1\r\nContent-Length: 4\r\nHost: localhost\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nDATA\r\n\r\n";
	int status = client->getRequest().doRequest(const_cast<char *>(req.c_str()), req.size());
	if (status != 200)
		Log().Get(logERROR) << "expecting status 200 got " << status;
	CGIExec cgi;
	std::string path = get_working_path();
	Log().setLevel(logDEBUG);
	FileDescriptor *resp = cgi.run("/usr/local/bin/ubuntu_cgi_tester", path, filename, *client);
	std::string response = readAllCGIResponse(resp->getFd());
	assertHeaderStatus(response, "200", name);
	close(resp->getFd());
	delete listener;
	delete client;
	delete resp;
	delete servers;
}

void testItoa()
{
	std::string res = ft_itoa(8080);
	assertStringEqual(res, std::string("8080"), "ft_itoa");
}

void testCGI()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__  << "\033[0m" << std::endl;
	testItoa();
	assertCGIFailed("/notfound.php", "404", "Status: 404");
	assertCGIFailed("/500.php", "500", "Status: 500");
	//assertCGISuccess("/test.bla", "Status: 200 php_info()");
}

#endif //WEBSERV_CGI_HPP