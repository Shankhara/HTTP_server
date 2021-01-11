#include "RespCGI.hpp"

RespCGI::RespCGI(Client &c, int fd):    Response(c.getRequest(), Client::getBuf(), CLIENT_BUFFER_SIZE),
										fd_(fd),
										respCursor_(0) {
	resp_.reserve(100000256); // : )
}


RespCGI::~RespCGI() {}

int RespCGI::readResponse() {
	nbytes_ = 0;
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
	size_t  headersEnd;
	bool	statusParsed = false;

	while ((nbytes_ = read(fd_, buf_, bufSize_)) > 0)
	{
		if (!statusParsed) {
			parseCGIStatus_(nbytes_);
			statusParsed = true;
		}else{
			resp_.append(buf_, nbytes_);
		}
	}
	if (nbytes_ < 0)
		throw RespException(500);
	headersEnd = resp_.find("\r\n\r\n", 0);
	if (headersEnd != std::string::npos)
	{
		unsigned long contentLength = resp_.size() - (headersEnd + 4);
		resp_.insert(headersEnd, "\r\nContent-Length: "+ ft_itoa(contentLength));
	}
}

void RespCGI::parseCGIStatus_(int nbytes) {
	if (nbytes < 11)
		throw RespException(500);
	std::string resp(buf_, nbytes);
	if (resp.rfind("Status: ", 0) == std::string::npos)
		statusCode_ = 200;
	else
		setStatusCode_(resp);
	size_t statusLineEnd = resp.find("\r\n", 0);
	if (statusLineEnd == std::string::npos)
		throw RespException(500);
	nbytes_ = 0;
	initHeaders();
	resp_.assign(buf_, nbytes_);
	resp_.append(resp.c_str() + statusLineEnd + 2);
}

void RespCGI::setStatusCode_(const std::string &resp) {
	std::string statusCode;
	statusCode.reserve(3);
	for (int i = 0; i < 3; i++)
	{
		if (resp.c_str()[8 + i] < '0' || resp.c_str()[8 + i] > '9')
			throw RespException(500);
		statusCode.push_back(buf_[i + 8]);
	}
	statusCode_ = ft_atoi(statusCode);
}
