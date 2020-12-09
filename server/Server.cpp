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
	FD_ZERO(&master_);
	fds_.reserve(FD_SETSIZE);
	instance_ = this;
}

void Server::run_()
{
	fd_set					conn_fds;

	FD_ZERO(&conn_fds);
	for (;;)
	{
		conn_fds = master_;
		if (select(fdmax_+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			Log().Get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				Log().Get(logDEBUG) << "server::run -> select got an event on fd " <<  i;
				Log().Get(logERROR) << "server::run ->  FD" <<  fds_[4]->getFd();
				fds_[i]->onEvent();
			}
		}
	}
}

void Server::start()
{
	Server::run_();
}

void Server::addFileDescriptor(FileDescriptor *fd) {
	FD_SET(fd->getFd(), &master_);
	if (fd->getFd() > fdmax_)
		fdmax_ = fd->getFd();
	Log().Get(logDEBUG) << "server: add " << fd->getFd() << " MAX_FD " << fdmax_;
	fds_[fd->getFd()] = fd;
}

void Server::deleteFileDescriptor(int fd) {
	Log().Get(logDEBUG) << "server: delete " << fd;
	close(fd);
	FD_CLR(fd, &master_);
}

Server *Server::getInstance()
{
	if (instance_ == 0)
		instance_ = new Server();
	return instance_;
}

