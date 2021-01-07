#include "RespTrace.hpp"

RespTrace::RespTrace(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	request_ = req_.getTracePayload();
}

RespTrace::~RespTrace() { }

void RespTrace::writeHeaders_()
{
		writeStatusLine_(statusCode_);
		writeBaseHeaders_();
		writeThisHeader_("Content-type", "message/html");
		writeHeadersEnd_();
}

int RespTrace::readResponse()
{
	nbytes_ = 0;

	if (headersBuilt_ == false)
		writeHeaders_();
	append_(request_); // TODO if request.size() > bufSize output will be truncated

	return nbytes_;
}

void RespTrace::build()
{

}
