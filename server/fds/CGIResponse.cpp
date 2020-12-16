#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, Client &client): client_(client), httpStatus_(false)
{
	fd_ = fd;
	Log().Get(logDEBUG) << "Creating CGIResponse: " << fd_;
}

CGIResponse::~CGIResponse()
{
	int status;

	pid_t result = waitpid(pid_, &status, WNOHANG);
	if (result == 0)
		kill(pid_, 9);
	close(fd_);
	Log().Get(logDEBUG) << "CGIResponse deleted " << fd_;
}

int CGIResponse::pipeToClient() {
	char 	buf[BUFFER_SIZE + 1];
	int		nbytes;

	if ((nbytes = read(fd_, buf, BUFFER_SIZE)) > 0)
	{
		if (!httpStatus_)
		{
			parseCGIStatus(buf, nbytes);
			httpStatus_ = true;
		}
		Log().Get(logDEBUG) << "CGIResponse::read > FD " << fd_ << " READ " << nbytes;
		if (send(client_.getFd(), buf, nbytes, 0) == -1)
		{
			Log().Get(logERROR) << "CGIResponse::send > FD " << fd_ << " SENT ERROR " << strerror(errno);
			return 0;
		}
	}
	return nbytes;
}

void CGIResponse::onEvent()
{
	if (pipeToClient() < 0)
		Log().Get(logDEBUG) << "CGIResponse > read error " << strerror(errno);
	Server::getInstance()->deleteFileDescriptor(client_.getFd());
}

pid_t CGIResponse::getPid() const {
	return pid_;
}

void CGIResponse::setPid(pid_t pid) {
	pid_ = pid;
}

void CGIResponse::parseCGIStatus(char *buf, int nbytes) {
	// TODO: dont use send?
	if (nbytes < 11 || strncmp(buf, "Status: ", 8) != 0)
	{
		send(client_.getFd(),"HTTP/1.1 500 Internal Server Error\r\n", 34, 0);
		return ;
	}
	//TODO: check if its a valid HTTP code
	std::string header = "HTTP/1.1 ";
	header.append(buf + 8, 3);
	header.append("\r\n");
	send(client_.getFd(), header.c_str(),header.length(), 0);
}
