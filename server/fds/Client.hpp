#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sys/socket.h>
#include "../Logger.hpp"
#include "../Server.hpp"
#include "FileDescriptor.hpp"
#include "../Request.hpp"
#include "../CGIExec.hpp"
#include "../parsing/Parsing.hpp"

#define MAX_CGI_FORKS 20


class Client: public FileDescriptor {

private:
	std::vector<Parsing::server> 		&servers_;
	FileDescriptor						*CGIResponse_;
	std::string							response_;
	Request								request_;

public:
	Client(int, std::vector<Parsing::server> &);
	virtual 						~Client();
	void 							constructRequest(char [], int);
	void 							sendResponse() const;
	void 							appendResponse(char [], int);
	std::string 					&getResponse();
	void 							setAddr(struct sockaddr_storage addr);
	void 							onEvent();
	Request 						&getRequest();
	std::vector<Parsing::server>	&getServers() const;
};


#endif //WEBSERV_CLIENT_HPP
