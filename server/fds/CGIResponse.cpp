#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, Client &client): client_(client) {
	fd_ = fd;
}

CGIResponse::~CGIResponse() {}

void CGIResponse::onEvent() {
	char 	buf[256];
	int		nbytes;

	while ((nbytes = read(fd_, buf, 255)) > 0)
		client_.appendResponse(buf, nbytes);
	if (nbytes < 0)
	{
		Log().Get(logDEBUG) << "CGIResponse > read error " << strerror(errno);
		client_.sendResponse();
		// TODO: improv deleteFileDescriptor will close on already close fd, maybe send on EOF is good enough?
		Server::getInstance()->deleteFileDescriptor(fd_);
	}
}
