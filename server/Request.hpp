#pragma once

#include <vector>
#include "Utils.hpp"
#include "Client.hpp"

#define LEN 100

class Client;

class Request
{
	private:
	std::string request_;
	std::vector<std::string> requestLine_;

	public:
	Request();
	Request(Client);
	~Request();
	
	Client client;
	std::vector<std::string> methods { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTION", "TRACE", "PATCH" };
	enum e_methods { GET, HEAD, POST, PUT, DELETE, OPTION, TRACE, PATCH };
	enum e_RequestLine { METHOD, URL, VERSION };

	int parse();
	int parseRequestLine();
	int checkMethod();
};
