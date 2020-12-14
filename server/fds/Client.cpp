#include "Client.hpp"

Client::Client(int fd, FileDescriptor &f): listener_(f) {
	fd_ = fd;
}

Client::~Client() {
	Log().Get(logDEBUG) << "Client deleted " << fd_;
}

void Client::onEvent()
{
	static char buf[256];
	int nbytes = recv(fd_, buf, sizeof(buf), 0);
	Log().Get(logDEBUG) << "Client" << listener_.getFd() << " client " << addr_.ss_family << " -> RECV " << nbytes;
	if (nbytes <= 0)
	{
		if (nbytes < 0)
		{
			Log().Get(logERROR) << "Client " << addr_.ss_family << "recv error" << strerror(errno);
			Server::getInstance()->deleteFileDescriptor(fd_);
			return ;
		}
	}
	constructRequest(buf, nbytes);
}

void Client::sendResponse() const
{
	Log().Get(logDEBUG) << "Client" << listener_.getFd() << " client " << addr_.ss_family << " -> sent " <<  response_;
	if (send(fd_, response_.c_str(), response_.length(), 0) == -1)
		Log().Get(logERROR) << "Listener" << listener_.getFd() << " client " << addr_.ss_family << "send error" << strerror(errno);
}

void Client::setAddr(struct sockaddr_storage addr) {
	addr_ = addr;
}

void Client::constructRequest(char buf[], int nbytes) {
	int result;
	if ((result = request_.appendRequest(buf, nbytes)) == 4)
	{
		CGIExec exec = CGIExec();
		CGIResponse *response = exec.run("/usr/bin/php-cgi", "/tmp", "/200.php", *this);
		if (response == 0)
			Log().Get(logERROR) << __FUNCTION__  << "WE SHOULD RETURN A 500 STATUS CODE";
		Server::getInstance()->addFileDescriptor(response);
	}else{
		Log().Get(logDEBUG) << __FUNCTION__  << " got result " << result;
	}
	//TODO: delete client
}

std::string &Client::getResponse()
{
	return response_;
}

void Client::appendResponse(char buf[], int nbytes) { // C'est la class Response qui va renvoyer la reponse prete.
	response_.append(buf, nbytes);					 // On y accedera comme ca : response.getResponseMsg();
}

FileDescriptor &Client::getListener() const {
	return listener_;
}

Request &Client::getRequest(){
	return request_;
}
