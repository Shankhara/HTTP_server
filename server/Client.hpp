#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>

class Client {

private:
	const int		fd_;
	struct sockaddr_storage addr_;
	std::string		response_;
	bool 			keepAlive_;

public:
	Client();
	Client(int);
	~Client();
	void				onDataReceived(char[], int);
	void 				setAddr(struct sockaddr_storage addr);
	const std::string	&getResponse() const;
	bool 				isKeepAlive() const;
};
#endif //WEBSERV_CLIENT_HPP
