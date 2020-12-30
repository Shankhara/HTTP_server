#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): \
Response( r, buf, bufSize), statusCode_(statusCode)
{
	fd_ = 0;
	Log().Get(logDEBUG) << __FUNCTION__  << " Generating response for " << req_.getReqTarget();
}

RespError::~RespError() {}

int RespError::readResponse()
{
	//TODO: #42
	nbytes_ = 0;
	if (fd_ == 0)
		writeErrorPage(statusCode_);
	return nbytes_;
}

