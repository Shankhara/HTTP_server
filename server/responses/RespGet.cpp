#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): RespFile(r, buf, bufSize)
{
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();
}

RespGet::~RespGet(){}

void RespGet::reachResource_()
{
	openFile_(O_RDONLY, 404);
}



void RespGet::writeHeaders_()
{
	initHeaders();
	writeContentType_(filePath_);
	writeContentLength_(fileSize_);
    if (acceptLangNegotiated_)
    {
        writeThisHeader_("Content-Location", filePath_);
    }
	writeHeadersEnd_();
}

void RespGet::build()
{
	setFilePath_();
	if (location_->autoindex == false || reqTarget_[reqTarget_.size() - 1] != '/')
	    reachResource_ ();
}

int RespGet::readResponse()
{
	nbytes_ = 0;

	if (fd_ == 0)
		return (writeAutoIndex_(location_->root + reqTarget_));
	else
	{
		if (!headersBuilt_)
			writeHeaders_();
		return read_();
	}
}

