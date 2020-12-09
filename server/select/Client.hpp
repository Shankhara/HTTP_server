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

class Client: public FileDescriptor {

private:
	FileDescriptor 		*listener_;
	const int		fd_;
	struct sockaddr_storage addr_;
	std::string		response_;
	bool 			keepAlive_;

public:
	Client();
	Client(int);
	~Client();
	void 				constructRequest(char [], int);
	void 				sendResponse();
	std::string 		&getResponse();
	void 				setAddr(struct sockaddr_storage addr);
	void				setListener(FileDescriptor &);
	void 				onEvent();
};
#endif //WEBSERV_CLIENT_HPP
