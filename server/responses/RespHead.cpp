#include "RespHead.hpp"

RespHead::RespHead(const Request &r, char *buf, unsigned int bufSize) : RespGet(r, buf, bufSize){}

RespHead::~RespHead() {}

void RespHead::append_(const std::string &str) {

	if (headersBuilt_)
		return ;
	Response::append_(str);
}

void RespHead::append_(const char str[], unsigned int size) {
	if (headersBuilt_)
		return ;
	Response::append_(str, size);
}

int RespHead::readResponse() {
	nbytes_ = 0;

	if (fd_ == 0)
		return (writeAutoIndex_(location_->root + reqTarget_));
	else
	{
		if (!headersBuilt_)
			writeHeaders_();
	}
	return nbytes_;
}

void RespHead::build() {
	RespGet::build();
}

