#pragma once

#include <string.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <errno.h>
//#include <sys/unistd.h>
#include <sys/fcntl.h>


class server
{
    private:
    std::string		name_;
	const char*		port_;
    int	sockfd_;
    struct addrinfo	*res_;
    void run_();
	void listen_();

	public:
    server();
    server(std::string, const char*);
	~server();
    
    void start();
};
