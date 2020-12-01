#include "server.hpp"

server::server(std::string serverName, const char* port = 0) : _name(serverName), _port(port), _sockfd(-1)
{
	memset(&hints, 0, sizeof hints);
}

server::~server() {
}

void server::listen_()
{
	int status;
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//Pour set une IP, enlever AI_PASSIVE et remplacer NULL
	if ((status = getaddrinfo(NULL, _port, &hints, &_res)))
	{
		std::cerr << "server:start -> error in getaddrinfo(), error code: " << status << std::endl;
	    exit(8);
	}

	if ((_sockfd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol)) == -1)
	{
		std::cerr << "server:start -> error in socket()\n";
	    exit(8);
    }

	if ((bind(_sockfd, _res->ai_addr, _res->ai_addrlen)) == -1)
	{
		std::cerr << "server:start -> error in bind()\n";
	    exit(8);
    }

	int yes = 1;
	// Pour contrer le "Add_ress already in use" error message
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
	    std::cerr << "server:start -> error in setsockopt()\n";
	    exit(8);
	} 

    if (listen(_sockfd, 20) == -1)
	{
	    std::cerr << "server:start -> error in listen()\n";
	    exit(8);
	}
}

void server::run_() {
	fd_set					master;
	fd_set					conn_fds;
	struct sockaddr_storage remoteaddr;
	socklen_t 				addrlen;
	int						newfd;
	int 					fdmax = _sockfd;
	int 					nbytes = 0;
	char					buf[256];
	FD_ZERO(&master);
	FD_ZERO(&conn_fds);
	FD_SET(_sockfd, &master);

	for (;;) {
		conn_fds = master;
		if (select(fdmax+1, &conn_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(4);
		}

		for (int i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &conn_fds)) {
				if (i == _sockfd) {
					addrlen = sizeof(remoteaddr);
					newfd = accept(_sockfd,
						(struct sockaddr *)(&remoteaddr), &addrlen);
					if (newfd == -1) {
						perror("client accept");
					} else {
						FD_SET(newfd, &master);
						if (newfd > fdmax) {
							fdmax = newfd;
						}
						std::cerr << "server::run -> new conn " << std::endl;
					}
				}else {
					if ((nbytes == recv(i, buf, sizeof(buf), 0)) <= 0) {
						if (nbytes == 0) {
							std::cerr << "server::run -> closed" << std::endl;
						} else {
							perror("client recv");
						}
						close(i);
						FD_CLR(i, &master);
					}
				}
			}
		}
	}
}


void server::start() {
	server::listen_();
	server::run_();
}
