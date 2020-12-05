#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include "Logger.hpp"

class Client {

private:
	int 			listenerId_;
	const int		fd_;
	struct sockaddr_storage addr_;
	std::string		response_;
	bool 			keepAlive_;

public:
	Client();
	Client(int);
	~Client();
	int					onDataReceived();
	void 				constructRequest(char [], int);
	void 				sendResponse();
	std::string 		&getResponse();
	void 				setAddr(struct sockaddr_storage addr);
	void				setListenerId(int listenerId);
};
#endif //WEBSERV_CLIENT_HPP
