#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): Response(r, buf, bufSize)
{
	statusCode_ = statusCode;
}

RespError::~RespError() {}

int RespError::readResponse()
{
	nbytes_ = 0;
	
	if (headersBuilt_ == true)
		return 0;

	writeFirstPart_();
	if (statusCode_ == 401)
		writeThisHeader_("WWW-Authenticate", "Basic realm=\"simple\"");
	if (statusCode_ == 405)
		writeAllow_();
	if (statusCode_ == 503)
		writeThisHeader_("Retry-After", "10");

	writeErrorBody(statusCode_);
	return nbytes_;
}

void RespError::build() {}

