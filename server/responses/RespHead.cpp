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
	return RespGet::readResponse();
}


RespHead::RespHead(const Request &r, char *buf, unsigned int bufSize) : RespGet(r, buf, bufSize){}

RespHead::~RespHead() {}
