#include <sys/socket.h>
#include <cstring>
#include "Client.hpp"

Client::Client(): fd_(-1) {}

Client::Client(int fd): fd_(fd)
{
	std::cerr << "New Client: " << fd_ << std::endl;
}

Client::~Client() {}

Client Client::operator=(const Client &o)
{
	fd_ = o.fd_;
	return *this;
}

void Client::onDataReceived(char buf[], int nbytes)
{
	if (send(fd_, buf, nbytes, 0) == -1)
		std::cerr << "server::run -> response sent error: " << strerror(errno) << " FD: " << fd_ << std::endl;
	else
		std::cerr << "server::run -> response SENT" << std::endl;
}






