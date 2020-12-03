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
#include "Client.hpp"
#include <vector>
#include <sys/fcntl.h>



class Server
{
private:
	Server();
	std::string			name_;
	const char*			port_;
	std::vector<Client>	clients_;
    int					sockfd_;
    int 				fdmax_;
	fd_set				master_;
    struct addrinfo		*res_;
    void 				run_();
	void 				listen_();

public:
    Server(std::string, const char*);
	~Server();

	void 		onClientConnect();
	void 		onClientDisconnect(int);
	void 		sendClientResponse(int clientSocket);
    void 		start();
};