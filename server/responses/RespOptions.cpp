#include "RespOptions.hpp"

RespOptions::RespOptions(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	statusCode_ = 200;
}

RespOptions::~RespOptions() { }

void RespOptions::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		writeThisHeader_("allow: ", req_.getHeaderAllow());
		writeHeadersEnd_();
	}
}

int RespOptions::readResponse()
{
	nbytes_ = 0;

	makeResponse_();

	return nbytes_;
}
