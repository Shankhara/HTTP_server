#include "RespHead.hpp"

void RespHead::append_(std::string str) {

	if (headersBuilt_)
		return ;
	Response::append_(str);
}

void RespHead::append_(char str[], unsigned int size) {
	if (headersBuilt_)
		return ;
	Response::append_(str, size);
}

int RespHead::readResponse() {
	nbytes_ = 0;
	if (fd_ == -1)
		return 0;
	if (fd_ == 0)
	{
		fd_ = -1;
		Parsing::location *location = req_.getLocation();
		if (req_.getLocation()->autoindex && req_.getReqTarget()[req_.getReqTarget().size() - 1] == '/')
			return (writeAutoIndex_(location->root + req_.getReqTarget()));
		openFile_(location);
		if (fd_ == -1)
			return (writeErrorPage(404));
	}
	return(readFile_());
}


RespHead::RespHead(const Request &r, char *buf, unsigned int bufSize) : RespGet(r, buf, bufSize){}

RespHead::~RespHead() {}
