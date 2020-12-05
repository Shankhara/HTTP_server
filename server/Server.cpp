#include "Server.hpp"

Server* Server::instance = 0;

Server::~Server() {
	for (int i = 0; i < fdmax_; i++)
	{
		if (FD_ISSET(i, &master_))
			close(i);
	}
}

Server::Server(): name_("webserv"), port_(80)
{
	clients_.reserve(FD_SETSIZE);
	for (int i = 0; i < FD_SETSIZE; i++)
		clients_.push_back(Client(i));
	instance = this;
}

void Server::listen_()
{
	struct sockaddr_in server;

	if ((sockfd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in socket()\n";
	    exit(8);
    }
	memset(&server, 0, sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ft_htonl(INADDR_ANY);
	server.sin_port = ft_htons(port_);
	if ((bind(sockfd_, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in bind() " << strerror(errno);
	    exit(8);
    }
    if (listen(sockfd_, FD_SETSIZE) == -1)
	{
    	Log().Get(logERROR) << "server:start -> error in listen() " << strerror(errno);
	    exit(8);
	}
}

void Server::run_()
{
	fd_set					conn_fds;

	FD_ZERO(&master_);
	FD_ZERO(&conn_fds);
	FD_SET(sockfd_, &master_);
	fdmax_ = sockfd_;
	for (;;)
	{
		conn_fds = master_;
		if (select(fdmax_+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			Log().Get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_;
			exit(4);
		}
		Log().Get(logDEBUG) << "server::run -> select UNLOCK";
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				if (i == sockfd_)
					onClientConnect();
				else if (clients_[i].onDataReceived() <= 0)
						onClientDisconnect(i);
			}
		}
	}
}

void Server::start()
{
	Log().Get(logINFO) << "Server " << name_ << " started on port " << port_ << " (maxconn: " << FD_SETSIZE << ")";
	Server::listen_();
	Server::run_();
}


void Server::onClientConnect() {
	socklen_t 				addrlen;
	struct sockaddr_storage remoteaddr;
	int						newfd;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(sockfd_, (struct sockaddr *)(&remoteaddr), &addrlen)) == -1)
	{
		Log().Get(logERROR) << "server::onClientConnect " << strerror(errno);
		exit(8);
	}

	FD_SET(newfd, &master_);
	if (newfd > fdmax_)
		fdmax_ = newfd;
	clients_[newfd].setAddr(remoteaddr);
}

void Server::onClientDisconnect(int fd_) {
	Log().Get(logDEBUG) << "ClientDisconnect: " << fd_;
	close(fd_);
	FD_CLR(fd_, &master_);
}

Server *Server::getInstance() {
	if (instance == 0)
	{
		instance = new Server();
	}
	return instance;
}

void Server::setName(const std::string &name) {
	name_ = name;
}

void Server::setPort(int port) {
	port_ = port;
}
