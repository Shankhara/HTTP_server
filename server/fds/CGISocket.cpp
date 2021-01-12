#include "CGISocket.hpp"

unsigned int CGISocket::instances = 0;

CGISocket::CGISocket(int fd, Client &client): client_(client)
{
	fd_ = fd;
	lastEventTimer_ = 0;
	Log::get(logDEBUG) << "Creating CGIResponse: " << fd_ << " From Client " << client_.getFd() << std::endl;
	instances++;
}

CGISocket::~CGISocket()
{
	int status;

	pid_t result = waitpid(pid_, &status, WNOHANG);
	if (result == 0)
		kill(pid_, 9);
	Log::get(logDEBUG) << "CGIResponse deleted " << fd_ << std::endl;
	instances--;
}


void CGISocket::onEvent()
{
	client_.setLastEventTimer();
	RespCGI response(client_, fd_);
	try {
		response.build();
		client_.sendResponse(&response);
	} catch (RespException &e) {
		RespError err(500, client_.getRequest(), Client::getBuf(), CLIENT_BUFFER_SIZE);
		client_.sendResponse(&err);
	}
}


void CGISocket::setPid(pid_t pid) {
	pid_ = pid;
}

