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
#include "../responses/RespGet.hpp"

#define MAX_CGI_FORKS 20
#define CLIENT_BUFFER_SIZE	32768


class Client: public FileDescriptor {

private:
	FileDescriptor						*CGIResponse_;
	std::string							response_;
	Request								request_;
	static char 						buf_[CLIENT_BUFFER_SIZE];
	void 								doResponse_();
	void 								sendErrorPage(int);

public:
	Client(int, std::vector<Parsing::server> &);
	virtual 						~Client();
	void 							constructRequest(char [], int);
	std::string 					&getResponse();
	void 							onEvent();
	Request							&getRequest();
};

#endif //WEBSERV_CLIENT_HPP
