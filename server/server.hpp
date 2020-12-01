#pragma once

#include <string.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>

class server
{
    private:
    std::string	_name;
	const char* _port;
    int	_sockfd;
    struct addrinfo	hints, *_res;

    public:
    server();
    server(std::string, const char*);
	~server();
    
    int start();
};
