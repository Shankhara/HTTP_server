#include "Client.hpp"

char Client::buf_[CLIENT_BUFFER_SIZE];

Client::Client(int fd, const Listener &l): request_(l.getServers()) {
	CGIResponse_ = 0;
	resp_ = 0;
	fd_ = fd;
	setLastEventTimer();
	Log::get(logDEBUG) << "Creating Client: " << fd_ << std::endl;
}

Client::~Client() {
	Log::get(logDEBUG) << "Client deleted: " << fd_ << std::endl;
	close(fd_);
	if (resp_ != 0)
		delete(resp_);
	if (CGIResponse_ != 0)
		Server::getInstance()->deleteFileDescriptor(CGIResponse_->getFd());
}

void Client::onEvent()
{
	setLastEventTimer();
	int nbytes = recv(fd_, buf_, CLIENT_BUFFER_SIZE - 1, 0);
	if (nbytes <= 0)
	{
		if (nbytes < 0)		
			Log::get(logERROR) << __FUNCTION__ << "Client " << fd_ << " recv error" << strerror(errno) << std::endl;
		else
			Log::get(logINFO) << __FUNCTION__ << "Client " << fd_ << " client closed connection" << std::endl;
		Server::getInstance()->deleteFileDescriptor(fd_);
		return ;
	}
	constructRequest(buf_, nbytes);
}

void Client::constructRequest(char buf[], int nbytes) {
	int statusCode;

	statusCode = request_.doRequest(buf, nbytes);
	Log::get(logDEBUG) << __FUNCTION__ << " Client: " << fd_ << " parsing status: " << statusCode << std::endl;
	if (statusCode >= 400)
	{
		RespError resp(statusCode, request_, buf_, CLIENT_BUFFER_SIZE);
		sendResponse_(&resp);
	}
	else if (request_.getLocation() != 0)
			doResponse_();
}

inline bool ends_with(std::string const & value, std::string const & ending)
{
	if (ending.size() > value.size()) return false;
	return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

bool Client::isFileCGI_(const Parsing::location *location, std::string filePath)
{
	for (size_t i = 0; i < location->cgi_extension.size(); i++)
	{
		if (ends_with(filePath, location->cgi_extension[i]))
			return true;
	}
	return false;
}

void Client::doResponse_() {
	if (request_.getLocation()->cgi_extension.empty() || !isFileCGI_(request_.getLocation(), request_.getReqTarget()))
	{
		if (request_.getStatusCode() == 200 || request_.getMethod() == "PUT" || request_.getMethod() == "POST" )
			doStaticFile_();
	}
	else if (request_.getStatusCode() == 200)
		doCGI_();
}

void Client::sendResponse_(Response *resp) {
	bool 	isSent = false;
	int 	nbytes;
	size_t	sentSize = 0;

	while ((nbytes = resp->readResponse()) > 0)
	{
		isSent = true;
		sentSize += nbytes;
		if ((nbytes = send(fd_, buf_, nbytes, 0)) < 0)
		{
			Log::get(logERROR) << " unable to send to client " << strerror(errno) << " nbytes: " << nbytes << std::endl;
			break ;
		}else if (nbytes == 0) {
			Log::get(logERROR) << " send returned 0" << std::endl;
			break ;
		}
	}
	if (isSent)
	{
		Log::get(logINFO) << "> fd: " << fd_ << " - " << resp->getStatusCode() << " - " << request_.getMethod() << " http://" << request_.getHeaderHost() << request_.getOriginalReqTarget() << " [" << sentSize << "]" << std::endl;
		Server::getInstance()->deleteFileDescriptor(fd_);
	}
}

void Client::responseFactory_() {
	// TODO: factory just like CPPdays to avoid if else branching?
	if (request_.getMethod() == "GET")
		resp_ = new RespGet(request_, buf_, CLIENT_BUFFER_SIZE);
	else if (request_.getMethod() == "POST")
		resp_ = new RespPost(request_, buf_, CLIENT_BUFFER_SIZE);
	else if (request_.getMethod() == "HEAD")
		resp_ = new RespHead(request_, buf_, CLIENT_BUFFER_SIZE);
	else if (request_.getMethod() == "TRACE")
		resp_ = new RespTrace(request_, buf_, CLIENT_BUFFER_SIZE);
	else if (request_.getMethod() == "DELETE")
		resp_ = new RespDelete(request_, buf_, CLIENT_BUFFER_SIZE);
	else if (request_.getMethod() == "PUT")
		resp_ = new RespPut(request_, buf_, CLIENT_BUFFER_SIZE);
	else
		resp_ = new RespError(400, request_, buf_, CLIENT_BUFFER_SIZE);
}

void Client::doStaticFile_() {
	if (resp_ == 0)
		responseFactory_();
	sendResponse_(resp_);
}

void Client::doCGI_() {
	if (CGIResponse_ != 0)
	{
		Log::get(logERROR) << __FUNCTION__  << " CGIResponse is already set for client " << fd_ << std::endl;
		return ;
	}
	if (CGISocket::instances > MAX_CGI_FORKS) {
		Log::get(logERROR) << __FUNCTION__ << "Too many CGIRunning, bounce this client: " << fd_ << std::endl;
		RespError resp(500, request_, buf_, CLIENT_BUFFER_SIZE);
		sendResponse_(&resp);
		Server::getInstance()->deleteFileDescriptor(fd_);
		return ;
	}
	CGIExec exec = CGIExec(*this);
	CGIResponse_ = exec.run();
	Server::getInstance()->addFileDescriptor(CGIResponse_);
	if (CGIResponse_ == 0) {
		RespError resp(500, request_, buf_, CLIENT_BUFFER_SIZE);
		sendResponse_(&resp);
	}
}

Request &Client::getRequest() {
	return request_;
}
