#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): Response(r, buf, bufSize)
{
	statusCode_ = statusCode;
}

RespError::~RespError() {}

int RespError::readResponse()
{
	nbytes_ = 0;
	
	if (headersBuilt_)
		return 0;

	writeStatusLine_(statusCode_);
	writeBaseHeaders_();
	writeContentType_("text/html");

	if (statusCode_ == 401)
		writeThisHeader_("WWW-Authenticate", "Basic realm=\"simple\"");
		//writeThisHeader_("WWW-Authenticate", "Basic realm=\"" + req_.getLocation()->auth_basic + "\""); // TODO #73
	if (statusCode_ == 405)
		writeAllow_();
	if (statusCode_ == 503)
		writeThisHeader_("Retry-After", "10");

	writeErrorBody(statusCode_);
	return nbytes_;
}

void RespError::build() {}

