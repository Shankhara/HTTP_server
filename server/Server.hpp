#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

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
#include <vector>
#include "Client.hpp"
#include "Logger.hpp"
#include "hton.hpp"

class Server
{
private:
	static Server		*instance;
	std::string			name_;
	int					port_;
	std::vector<Client>	clients_;
    int					sockfd_;
    int 				fdmax_;
	fd_set				master_;
	Server();
    void 				run_();
	void 				listen_();
	uint32_t 			htonl_(uint32_t);
	uint16_t 			htons_(uint16_t);

public:
	~Server();
	static Server		*getInstance();
	void 		onClientConnect();
	void 		onClientDisconnect(int);
    void 		start();
	void		setName(const std::string &name);
	void 		setPort(int port);
};

#endif