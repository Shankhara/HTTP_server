#include "Client.hpp"

char Client::buf_[CLIENT_BUFFER_SIZE];

Client::Client(int fd, std::vector<Parsing::server> &s): request_(s) {
	CGIResponse_ = 0;
	fd_ = fd;
	setLastEventTimer();
	Log().Get(logDEBUG) << "Creating Client: " << fd_;
}

Client::~Client() {
	Log().Get(logDEBUG) << "Client deleted: " << fd_;
	close(fd_);
	if (CGIResponse_ != 0)
		Server::getInstance()->deleteFileDescriptor(CGIResponse_->getFd());
}

void Client::onEvent()
{
	setLastEventTimer();
	int nbytes = recv(fd_, buf_, CLIENT_BUFFER_SIZE, 0);
	Log().Get(logDEBUG) << __FUNCTION__  << " Client" << fd_ << " -> RECV " << nbytes;
	if (nbytes <= 0)
	{
		if (nbytes < 0)		
			Log().Get(logERROR) << __FUNCTION__ << "Client " << fd_ << " recv error" << strerror(errno);
		else
			Log().Get(logINFO) << __FUNCTION__ << "Client " << fd_ << " client closed connection";
		Server::getInstance()->deleteFileDescriptor(fd_);
		return ;
	}
	constructRequest(buf_, nbytes);
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

void Client::constructRequest(char buf[], int nbytes) {
	int status;

	status = request_.doRequest(buf, nbytes);
	Log().Get(logDEBUG) << __FUNCTION__ << " Client: " << fd_ << " parsing status: " << status;
	if (status == 100)
		return ;
	else if (status == 200)
		doResponse_();
	else
	{
		Log().Get(logERROR) << __FUNCTION__  << "Client: " << fd_ << " Parse Error code: " << status;
		sendErrorPage(400);
	}
}

void Client::doResponse_() {
	unsigned int nbytes;
	if (request_.getLocation()->cgi_extension.size() == 0 || !ends_with(request_.getReqTarget(), request_.getLocation()->cgi_extension[0]))
	{
		RespGet response(request_, buf_, CLIENT_BUFFER_SIZE);
		while ((nbytes = response.readResponse()) > 0)
		{
			if (send(fd_, buf_, nbytes, 0) < 0)
			{
				Log().Get(logERROR) << " unable to send to client " << strerror(errno) << " nbytes: " << nbytes;
				break ;
			}
		}
		Server::getInstance()->deleteFileDescriptor(fd_);
		return ;
	}
	if (CGIResponse_ != 0) {
		Log().Get(logERROR) << " parse returned 200 but CGIResponse was already set: "
							<< request_.getRequest();
		return ;
	}
	CGIExec exec = CGIExec();
	CGIResponse_ = exec.run(*this);
	Server::getInstance()->addFileDescriptor(CGIResponse_);
	if (CGIResponse_ == 0)
		send(fd_, "HTTP/1.1 500 Internal Server Error\r\n", 36, 0);
	/*if (CGIResponse::instances > MAX_CGI_FORKS)
	{
		Log().Get(logERROR) << __FUNCTION__  << "Too many CGIRunning, bounce this client: " << fd_;
		send(fd_, "HTTP/1.1 500 Internal Server Error\r\n", 36, 0);
		Server::getInstance()->deleteFileDescriptor(fd_);
		return ;
	}*/
}

void Client::sendErrorPage(int statusCode) {
	RespGet err(request_, buf_, CLIENT_BUFFER_SIZE);
	unsigned int nbytes = err.writeErrorPage(statusCode);
	send(fd_, buf_, nbytes, 0);
	Server::getInstance()->deleteFileDescriptor(fd_);
}

std::string &Client::getResponse()
{
	return response_;
}

void Client::appendResponse(char buf[], int nbytes) { 
	response_.append(buf, nbytes);					 
}

Request &Client::getRequest(){
	return request_;
}

