#include "Client.hpp"

Client::Client(): fd_(-1), keepAlive_(false) {}

Client::Client(int fd): fd_(fd), keepAlive_(false){}

Client::~Client() {}

int Client::onDataReceived()
{
	int nbytes = recv(fd_, buf_, sizeof(buf_), 0);
	std::cerr << "client " << addr_.ss_family << " -> RECV " << nbytes << " Keepalive " << keepAlive_ << std::endl;
	if (nbytes <= 0)
	{
		if (nbytes < 0)
			std::cerr << "client " << addr_.ss_family << "recv error" << strerror(errno) << std::endl;
		return (nbytes);
	}
	constructRequest(buf_, nbytes);
	if (response_.length() > 0)
		sendResponse();
	return (nbytes);
}

void Client::sendResponse()
{
	if (send(fd_, response_.c_str(), response_.length(), 0) == -1)
		std::cerr << "client " << addr_.ss_family << "send error" << strerror(errno) << std::endl;
}


void Client::setAddr(struct sockaddr_storage addr) {
	addr_ = addr;
}

void Client::constructRequest(char buf[], int nbytes) {
	response_.append(buf, nbytes);
}

std::string &Client::getResponse() {
	return response_;
}







