#include "Server.hpp"

Server* Server::instance_ = 0;

Server::~Server() {
	halt_();
}

Server::Server()
{
	fdmax_ = 0;
	shutdown_ = false;
	FD_ZERO(&master_);
	fds_.reserve(FD_SETSIZE);
}

void Server::run_()
{
	struct timeval tv 		= {5, 0};
	unsigned long 			lastGC = getTime();
	unsigned long 			cur;

	FD_ZERO(&conn_fds_);
	while (!shutdown_)
	{
		conn_fds_ = master_;
		if (select(fdmax_ + 1, &conn_fds_, NULL, NULL, &tv) == -1)
		{
			if (errno != EINTR)
				Log::get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_ << std::endl;
			return ;
		}
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds_))
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
	unwatch(fd);
	delete fds_[fd];
}

Server *Server::getInstance()
{
	if (instance_ == 0)
		instance_ = new Server();
	return instance_;
}

void Server::stop() {
	shutdown_ = true;
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

void Server::unwatch(int fd) {
	FD_CLR(fd, &master_);
	FD_CLR(fd, &conn_fds_);
}


void Server::halt_() {
	for (int i = 0; i <= fdmax_; i++)
	{
		if (FD_ISSET(i, &master_))
		{
			Log::get(logDEBUG) << "Unallocating FileDescriptor: " << i << std::endl;
			delete fds_[i];
		}
	}
	std::cerr.clear();
	std::cerr << "\b\b";
	Log::get(logINFO) << "Webserv exiting gracefully (FD_MAX: " << fdmax_ << ")" << std::endl;
}

bool Server::isRunning() {
	return !shutdown_;
}

