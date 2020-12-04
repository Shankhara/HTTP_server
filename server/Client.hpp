#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>

class Client {

private:
	const int		fd_;
	struct sockaddr_storage addr_;
	std::string		response_;
	bool 			keepAlive_;
	char			buf_[256];

public:
	Client();
	Client(int);
	~Client();
	int					onDataReceived();
	void 				constructRequest(char [], int);
	void 				sendResponse();
	std::string 		&getResponse();
	void 				setAddr(struct sockaddr_storage addr);
};
#endif //WEBSERV_CLIENT_HPP
