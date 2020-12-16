#include "Client.hpp"

Client::Client(int fd, Parsing::servers &s): server_(s) {
	CGIResponse_ = 0;
	fd_ = fd;
	Log().Get(logDEBUG) << "Creating Client: " << fd_;
}

Client::~Client() {
	Log().Get(logDEBUG) << "Client deleted " << fd_;
	close(fd_);
	if (CGIResponse_ != 0)
		Server::getInstance()->deleteFileDescriptor(CGIResponse_->getFd());
}

void Client::onEvent()
{
	static char buf[1024];
	int nbytes = recv(fd_, buf, sizeof(buf), 0);
	Log().Get(logDEBUG) << __FUNCTION__  << " Client" << fd_ << " -> RECV " << nbytes;
	if (nbytes <= 0)
	{
		if (nbytes < 0)
		{
			Log().Get(logERROR) << __FUNCTION__ << "Client " << fd_ << " recv error" << strerror(errno);
			Server::getInstance()->deleteFileDescriptor(fd_);
			return ;
		}
	}
	constructRequest(buf, nbytes);
}

void Client::sendResponse() const
{
	const char* responsePtr = response_.data();
	std::size_t responseSize = response_.size();
	int nbytes;

	while (responseSize > 0)
	{
		nbytes = send(fd_, responsePtr, responseSize, 0);
		if (nbytes == -1)
		{
			Log().Get(logERROR) << __FUNCTION__ << " Unable to send response " << strerror(errno);
			break ;
		}
		Log().Get(logDEBUG) << __FUNCTION__  << ">  Client" << fd_ << " -> sent NBYTES: " << nbytes;
		responsePtr += nbytes;
		responseSize -= nbytes;
	}
}

void Client::setAddr(struct sockaddr_storage addr) {
	addr_ = addr;
}

void Client::constructRequest(char buf[], int nbytes) {
	int result;

	if ((result = request_.appendRequest(buf, nbytes)) == 0)
	{
		CGIExec exec = CGIExec();
		CGIResponse_ = exec.run("/usr/bin/php-cgi", "/usr/local/wordpress", "/index.php", *this);
		if (CGIResponse_ == 0)
			Log().Get(logERROR) << __FUNCTION__  << "WE SHOULD RETURN A 500 STATUS CODE";
		Server::getInstance()->addFileDescriptor(CGIResponse_);
	}else{
		Log().Get(logERROR) << __FUNCTION__  << " TCP RST, We should send a 400 Response instead. Parse Error code: " << result << " REQ BODY: " << request_.request_;
		Server::getInstance()->deleteFileDescriptor(fd_);
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

Request &Client::getRequest(){
	return request_;
}

Parsing::servers &Client::getServer() const {
	return server_;
}
