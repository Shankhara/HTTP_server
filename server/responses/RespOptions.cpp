#include "RespOptions.hpp"

RespOptions::RespOptions(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize) { }

RespOptions::~RespOptions() { }

void RespOptions::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		writeBaseHeaders_();
		writeAllow_();
		writeHeadersEnd_();
	}
}

int RespOptions::readResponse()
{
	nbytes_ = 0;

	makeResponse_();

	return nbytes_;
}
