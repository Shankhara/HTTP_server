#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, Client &client): client_(client) {
	fd_ = fd;
	client_.setCgiResponse(this);
}

CGIResponse::~CGIResponse() {
	client_.setCgiResponse(0);
}

int CGIResponse::readPipe() {
	char 	buf[256];
	int		nbytes;

	while ((nbytes = read(fd_, buf, 255)) > 0)
	{
		Log().Get(logDEBUG) << "CGIResponse::read > FD " << fd_ << " READ " << nbytes;
		client_.appendResponse(buf, nbytes);
	}
	return nbytes;
}

void CGIResponse::onEvent() {
	if (readPipe() <= 0)
	{
		Log().Get(logDEBUG) << "CGIResponse > read error " << strerror(errno);
		client_.sendResponse();
		// TODO: improv deleteFileDescriptor will close an already close fd, maybe send on EOF is good enough?
		Server::getInstance()->deleteFileDescriptor(fd_);
	}
}

pid_t CGIResponse::getPid() const {
	return pid_;
}

void CGIResponse::setPid(pid_t pid) {
	pid_ = pid;
}
