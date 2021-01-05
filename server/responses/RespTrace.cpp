#include "RespTrace.hpp"

RespTrace::RespTrace(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	request_ = req_.getTracePayload();
}

RespTrace::~RespTrace() { }

void RespTrace::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		writeThisHeader_("Content-type", "message/html");
		writeHeadersEnd_();
	}
}

int RespTrace::readResponse()
{
	nbytes_ = 0;

	makeResponse_();
	append_(request_);

	return nbytes_;
}
