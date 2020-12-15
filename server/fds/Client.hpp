#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include "../Logger.hpp"
#include "../Server.hpp"
#include "FileDescriptor.hpp"
#include "../Request.hpp"
#include "../CGIExec.hpp"
#include "../parsing/Parsing.hpp"


class Client: public FileDescriptor {

private:
	Parsing::servers 		&server_;
public:
	Parsing::servers &getServer() const;

private:
	struct 					sockaddr_storage addr_;
	std::string				response_;
	Request					request_;

public:
	Client(int, Parsing::servers &);
	virtual ~Client();
	void 				constructRequest(char [], int);
	void 				sendResponse() const;
	void 				appendResponse(char [], int);
	std::string 		&getResponse();
	void 				setAddr(struct sockaddr_storage addr);
	void 				onEvent();
	Request 			&getRequest();
};


#endif //WEBSERV_CLIENT_HPP
