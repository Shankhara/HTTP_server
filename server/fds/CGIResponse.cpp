#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, Client &client): client_(client)
{
	fd_ = fd;
}

CGIResponse::~CGIResponse()
{
	int status;

	pid_t result = waitpid(pid_, &status, WNOHANG);
	if (result == 0)
		kill(pid_, 9);
}

int CGIResponse::readPipe() {
	char 	buf[256];
	int		nbytes;

	while ((nbytes = read(fd_, buf, 255)) > 0)
	{
		//Log().Get(logDEBUG) << "CGIResponse::read > FD " << fd_ << " READ " << nbytes;
		client_.appendResponse(buf, nbytes);
	}
	return nbytes;
}

void CGIResponse::onEvent() {
	int status;
	if ((status = readPipe()) <= 0)
	{
		if (status < 0)
			Log().Get(logDEBUG) << "CGIResponse > read error " << strerror(errno);
		client_.sendResponse();
		// TODO: improv deleteFileDescriptor will close an already close fd, maybe send on EOF is good enough?
		Server::getInstance()->deleteFileDescriptor(client_.getFd());
		Server::getInstance()->deleteFileDescriptor(fd_);
	}
}

pid_t CGIResponse::getPid() const {
	return pid_;
}

void CGIResponse::setPid(pid_t pid) {
	pid_ = pid;
}
