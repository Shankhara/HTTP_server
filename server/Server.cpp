#include "Server.hpp"

Server* Server::instance_ = 0;

Server::~Server() {
	Log::get(logINFO) << "FD_MAX at shutdown: " << fdmax_ << std::endl;
	stop();
}

Server::Server()
{
	fdmax_ = 0;
	FD_ZERO(&master_);
	fds_.reserve(FD_SETSIZE);
}

void Server::run_()
{
	fd_set					conn_fds;
	struct timeval tv 		= {5, 0};
	unsigned long 			lastGC = getTime();
	unsigned long 			cur;

	FD_ZERO(&conn_fds);
	for (;;)
	{
		conn_fds = master_;
		if (select(fdmax_ + 1, &conn_fds, NULL, NULL, &tv) == -1)
		{
			Log::get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_ << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				Log::get(logDEBUG) << "server::run -> select got an event on fd " <<  i << std::endl;
				fds_[i]->onEvent();
			}
		}
		if (((cur = getTime()) - lastGC) > 4 * 1000)
		{
			garbageCollector_();
			lastGC = cur;
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
	Log::get(logDEBUG) << __FUNCTION__ << " " <<  fd->getFd() << " MAX_FD " << fdmax_ << std::endl;
	fds_[fd->getFd()] = fd;
}

void Server::deleteFileDescriptor(int fd) {
	Log::get(logDEBUG) << __PRETTY_FUNCTION__  << " " << fd << std::endl;
	if (close(fd) == -1)
		Log::get(logERROR) << __PRETTY_FUNCTION__  << " unable to close " << strerror(errno) << std::endl;
	FD_CLR(fd, &master_);
	delete fds_[fd];
}

Server *Server::getInstance()
{
	if (instance_ == 0)
		instance_ = new Server();
	return instance_;
}

void Server::stop() {
	Log::get(logDEBUG) << "Server::stop()" << std::endl;
	for (int i = 0; i <= fdmax_; i++)
	{
		if (FD_ISSET(i, &master_))
		{
			Log::get(logDEBUG) << "deleting " << i << std::endl;
			delete fds_[i];
		}
	}
}

void Server::garbageCollector_()
{
	for (int i = 0; i <= fdmax_; i++)
	{
		if (FD_ISSET(i, &master_))
		{
			if (fds_[i]->getLastEventTimer() > 0 && (getTime() - fds_[i]->getLastEventTimer()) > READ_TIMEOUT * 1000)
			{
				//Log::get(logDEBUG) << "FD: " << i << " > Timeout after " << READ_TIMEOUT << " sec" << std::endl;
				deleteFileDescriptor(i);
			}
		}
	}
}

