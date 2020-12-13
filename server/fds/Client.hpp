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
#include <signal.h>
#include "FileDescriptor.hpp"


class Client: public FileDescriptor {

private:
	FileDescriptor 			&listener_;
	struct sockaddr_storage addr_;
	std::string				response_;
	bool 					keepAlive_;

public:
	Client(int, FileDescriptor &);
	virtual ~Client();
	void 				constructRequest(char [], int);
	void 				sendResponse() const;
	void 				appendResponse(char [], int);
	std::string 		&getResponse();
	void 				setAddr(struct sockaddr_storage addr);
	void 				onEvent();
	FileDescriptor 		&getListener() const;
};


#endif //WEBSERV_CLIENT_HPP