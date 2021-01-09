#include "RespOptions.hpp"

RespOptions::RespOptions(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize) { }

RespOptions::~RespOptions() { }

void RespOptions::makeResponse_()
{
	if (!headersBuilt_)
    {
		initHeaders();
        writeAllow_ ();
        writeContentLength_ (0);
        writeHeadersEnd_ ();
        writeHeadersEnd_ ();
    }
}

int RespOptions::readResponse()
{
	nbytes_ = 0;
	if (headersBuilt_ == false)
		makeResponse_();
	return nbytes_;
}
void RespOptions::build() {}