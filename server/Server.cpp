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

void Server::listen_(FileDescriptor &listener)
{

}

void Server::run_()
{
	fd_set					conn_fds;

	FD_ZERO(&master_);
	FD_ZERO(&conn_fds);
	for (unsigned long i = 0; i < fds_.size(); i++)
		FD_SET(fds_[i].getFd(), &master_);
	fdmax_ = fds_[fds_.size() - 1].getFd();
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
				fds_[i].onEvent();
		}
	}
}

void Server::start()
{
	for (unsigned long i = 0; i < fds_.size(); i++)
		Server::listen_(fds_[i]);
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



void Server::addListener(const std::string &name, const std::string &ip, int port)
{
	fds_.push_back(Listener(0, inet_addr(ip.c_str()), port, name));
	Log().Get(logINFO) << name << " started on port " << ip << ":" << port << " (maxconn: " << FD_SETSIZE << ")";
}

void Server::addFileDescriptor(const FileDescriptor &fd) {
	fds_.push_back(fd);
}

