#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): \
Response( r, buf, bufSize), statusCode_(statusCode)
{
	fd_ = 0;
}

RespError::~RespError() {}

int RespError::readResponse()
{
	nbytes_ = 0;
	if (fd_ == 0)
		writeErrorPage(statusCode_);
	return nbytes_;
}

