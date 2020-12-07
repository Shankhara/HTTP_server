#include "Server.hpp"

Server* Server::instance_ = 0;

Server::~Server() {
	for (int i = 0; i < fdmax_; i++)
	{
		if (FD_ISSET(i, &master_))
			close(i);
	}
}

Server::Server()
{
	clients_.reserve(FD_SETSIZE);
	for (int i = 0; i < FD_SETSIZE; i++)
		clients_.push_back(Client(i));
	instance_ = this;
}

void Server::listen_(struct s_listener &listener)
{
	struct sockaddr_in server;

	if ((listener.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in socket()\n";
	    exit(EXIT_FAILURE);
    }
	memset(&server, 0, sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = listener.ipv4;
	server.sin_port = htons_(listener.port);
	if ((bind(listener.fd, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in bind() " << strerror(errno);
	    exit(EXIT_FAILURE);
    }
    if (listen(listener.fd, FD_SETSIZE) == -1)
	{
    	Log().Get(logERROR) << "server:start -> error in listen() " << strerror(errno);
	    exit(EXIT_FAILURE);
	}
}

void Server::run_()
{
	fd_set					conn_fds;

	FD_ZERO(&master_);
	FD_ZERO(&conn_fds);
	for (unsigned long i = 0; i < listeners_.size(); i++)
		FD_SET(listeners_[i].fd, &master_);
	fdmax_ = listeners_[listeners_.size() - 1].fd;
	for (;;)
	{
		conn_fds = master_;
		if (select(fdmax_+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			Log().Get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_;
			exit(EXIT_FAILURE);
		}
		Log().Get(logDEBUG) << "server::run -> select UNLOCK";
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				if (isListener_(i))
					onClientConnect(i);
				else if (clients_[i].onDataReceived() <= 0)
						onClientDisconnect(i);
			}
		}
	}
}

void Server::start()
{
	for (unsigned long i = 0; i < listeners_.size(); i++)
		Server::listen_(listeners_[i]);
	Server::run_();
}


void Server::onClientConnect(int listener)
{
	socklen_t 				addrlen;
	struct sockaddr_storage remoteaddr;
	int						newfd;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(listener, (struct sockaddr *)(&remoteaddr), &addrlen)) == -1)
	{
		Log().Get(logERROR) << "server::onClientConnect " << strerror(errno);
		exit(8);
	}
	FD_SET(newfd, &master_);
	if (newfd > fdmax_)
		fdmax_ = newfd;
	clients_[newfd].setAddr(remoteaddr);
	clients_[newfd].setListenerId(listener);
}

void Server::onClientDisconnect(int fd_) {
	Log().Get(logDEBUG) << "ClientDisconnect: " << fd_;
	close(fd_);
	FD_CLR(fd_, &master_);
}

Server *Server::getInstance()
{
	if (instance_ == 0)
		instance_ = new Server();
	return instance_;
}

uint16_t Server::htons_(uint16_t hostshort)
{
	long ui = 0;

	ui |= (hostshort & 0xFF) << 8;
	ui |= (hostshort & 0xFF00) >> 8;
	return (ui);
}

bool Server::isListener_(int fd)
{
	for (unsigned long i = 0; i < listeners_.size(); i++)
	{
		if (listeners_[i].fd == fd)
			return true;
	}
	return false;
}

void Server::addListener(const std::string &name, const std::string &ip, int port)
{
	struct s_listener listener;
	listener.name = name;
	listener.port = port;
	listener.ipv4 = inet_addr(ip.c_str());
	listeners_.push_back(listener);
	Log().Get(logINFO) << name << " started on port " << ip << ":" << port << " (maxconn: " << FD_SETSIZE << ")";
}

