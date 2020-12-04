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

	int parse();
	int parseRequestLine();
	int checkMethod();
};
