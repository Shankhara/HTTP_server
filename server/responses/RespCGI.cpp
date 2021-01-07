#include "RespCGI.hpp"

RespCGI::RespCGI(Client &c, int fd):    Response(c.getRequest(), Client::getBuf(), CLIENT_BUFFER_SIZE),
										fd_(fd),
										respCursor_(0) {
	resp_.reserve(100000256);
}


RespCGI::~RespCGI() {}

int RespCGI::readResponse() {
	unsigned int remaining = resp_.size() - respCursor_;
	unsigned int readSize;
	if (remaining > bufSize_)
		readSize = bufSize_;
	else
		readSize = remaining;
	append_(resp_.c_str() + respCursor_, readSize);
	respCursor_ += readSize;
	return readSize;
}

void RespCGI::build() {
	int		nbytes;
	size_t  headersEnd;
	bool	statusParsed = false;

	while ((nbytes = read(fd_, buf_, bufSize_)) > 0)
	{
		if (!statusParsed) {
			parseCGIStatus_(nbytes);
			statusParsed = true;
		}else{
			resp_.append(buf_, nbytes);
		}
	}
	if (nbytes < 0) {
		writeErrorPage(500);
		return ;
	}
	headersEnd = resp_.find("\r\n\r\n", 0);
	if (headersEnd != std::string::npos)
	{
		unsigned long contentLength = resp_.size() - (headersEnd + 4);
		resp_.insert(headersEnd, "\r\nContent-Length: "+ ft_itoa(contentLength));
		Log::get(logINFO) << "LENGTH: " << contentLength << std::endl;
	}
}

void RespCGI::parseCGIStatus_(int nbytes) {
	if (nbytes < 11 || strncmp(buf_, "Status: ", 8) != 0)
	{
		resp_ = "HTTP/1.1 200 OK\r\n";
		resp_.append(buf_, nbytes);
		return ;
	}
	buf_[nbytes] = '\0';
	resp_ = "HTTP/1.1 ";
	resp_.append(buf_ + 8, 3);
	resp_.append(" OK\r\n");
	resp_.append("Connection: Close\r\n");
	resp_.append("Server: " + std::string(WEBSERV_ID) + "\r\n");
	std::string resp(buf_, nbytes);
	size_t crlf = resp.find("\r\n", 0);
	if (crlf == std::string::npos || crlf + 2 > static_cast<size_t>(nbytes))
		crlf = 0;
	resp_.append(buf_ + crlf + 2);
}
