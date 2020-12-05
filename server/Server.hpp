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

struct s_listener {
	int			port;
	int			fd;
	std::string name;
	uint32_t	ipv4;
};

class Server
{
private:
	static Server					*instance;
	std::vector<Client>				clients_;
    std::vector<struct s_listener>	listeners_;
    int 							fdmax_;
	fd_set							master_;
	Server();
    void 				run_();
	void 				listen_(struct s_listener &);
	bool        		isListener_(int fd);
	uint32_t 			htonl_(uint32_t);
	uint16_t 			htons_(uint16_t);

public:
	~Server();
	static Server		*getInstance();
	void 		onClientConnect(int);
	void 		onClientDisconnect(int);
    void 		start();
	void		addListener(const std::string &name, const std::string &ip, int port);
};

#endif