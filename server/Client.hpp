#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>

class Client {

private:
	int		fd_;

public:
	Client();
	Client(int);
	~Client();
	Client operator=(const Client &);
	void		onDataReceived(char[], int);
};


#endif //WEBSERV_CLIENT_HPP
