#include "Client.hpp"

Client::Client(): fd_(-1) {}

Client::Client(int fd): fd_(fd){}

Client::~Client() {}

void Client::onDataReceived(char buf[], int nbytes)
{
	std::cerr << "RECV" << fd_ << std::endl;
	response_.append(buf, nbytes);
}

void Client::setAddr(struct sockaddr_storage addr) {
	addr_ = addr;
}

const std::string &Client::getResponse() const {
	return response_;
}

bool Client::isKeepAlive() const {
	return keepAlive_;
}







