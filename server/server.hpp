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

class server
{
    private:
    std::string	_name;
	const char* _port;
    int	_sockfd;
    struct addrinfo	hints, *_res;
    void run_();
	void listen_();

public:
    server();
    server(std::string, const char*);
	~server();
    
    void start();
};
