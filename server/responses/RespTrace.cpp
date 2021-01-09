#include "RespTrace.hpp"

RespTrace::RespTrace(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	cursor_ = 0;
	request_ = req_.getTracePayload();
}

RespTrace::~RespTrace() { }

void RespTrace::writeHeaders_()
{
	initHeaders();
    writeThisHeader_("Content-type", "message/html");
    writeContentLength_(request_.size());
	writeHeadersEnd_();
}

int RespTrace::readResponse()
{
	nbytes_ = 0;
	if (headersBuilt_ == false)
		writeHeaders_();
	if (request_.size() - cursor_ > 0)
	{
		size_t readSize;
		if (request_.size() >= bufSize_ - nbytes_)
			readSize = bufSize_ - nbytes_;
		else
			readSize = request_.size();
		append_(request_.c_str() + cursor_, readSize);
		cursor_ += readSize;
		buf_[nbytes_] = '\0';
	}
	return nbytes_;
}

void RespTrace::build(){}
