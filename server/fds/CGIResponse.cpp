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
	client_.sendResponse();
	if (nbytes < 0)
		Log().Get(logERROR) << "CGIResponse > read error " << strerror(errno);
	// TODO: delete this :<
	delete this;
}
