#include "CGISocket.hpp"

unsigned int CGISocket::instances = 0;

CGISocket::CGISocket(int fd, Client &client): client_(client), httpStatus_(false)
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
	close(fd_);
	Log::get(logDEBUG) << "CGIResponse deleted " << fd_ << std::endl;
	instances--;
}

void CGISocket::appendContentLenght_() {
	size_t headerPos = resp_.find("\r\n\r\n", 0);
	if (headerPos == 0 || headerPos > resp_.size())
		resp_.append("Content-Length: 0");
	else{
		size_t bodySize = resp_.size() - (headerPos + 4);
		std::string itoa = ft_itoa(bodySize);
		Log::get(logDEBUG) << " HEADERPOS " << headerPos << "resp_ " << resp_.size() << std::endl;
		resp_.insert(headerPos, "Content-Length: " + itoa);
	}
}

int CGISocket::readCGIResponse() {
	char		buf[BUFFER_SIZE + 1];
	int			nbytes;

	if ((nbytes = read(fd_, buf, BUFFER_SIZE)) > 0)
	{
		if (!httpStatus_)
		{
			parseCGIStatus(buf, nbytes);
			httpStatus_ = true;
		}else{
			Log::get(logDEBUG) << "CGIResponse::read > FD " << fd_ << " READ " << nbytes << std::endl;
			resp_.append(buf, nbytes);
		}
	}
	return nbytes;
}

void CGISocket::onEvent()
{
	client_.setLastEventTimer();
	int nbytes = readCGIResponse();
	if (nbytes > 0)
		return ;
	else if (nbytes == 0) {
		appendContentLenght_();
		Log::get(logDEBUG) << "NBYTES " << resp_.size() << std::endl;
		send(client_.getFd(), resp_.c_str(), resp_.size(), 0);
	}
	else
		Log::get(logDEBUG) << "CGIResponse > read error " << strerror(errno) << std::endl;
	Server::getInstance()->deleteFileDescriptor(client_.getFd());
}


void CGISocket::setPid(pid_t pid) {
	pid_ = pid;
}

void CGISocket::parseCGIStatus(char *buf, int nbytes) {
	buf[nbytes] = '\0';
	if (nbytes < 11 || strncmp(buf, "Status: ", 8) != 0)
	{
		resp_ = "HTTP/1.1 200 OK\r\n";
		resp_.append(buf, nbytes);
		return ;
	}
	resp_ = "HTTP/1.1 ";
	resp_.append(buf + 8, 3);
	resp_.append(" OK\r\n");
	size_t crlf = std::string(buf, nbytes).find("\r\n", 0);
	if (crlf == std::string::npos || crlf + 2 > static_cast<size_t>(nbytes))
		crlf = 0;
	resp_.append(buf + crlf + 2);
}
