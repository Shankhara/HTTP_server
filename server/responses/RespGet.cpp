#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): RespFile(r, buf, bufSize)
{
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();
}

RespGet::~RespGet(){}

void RespGet::reachResource_()
{
	struct stat st;

	if (stat(filePath_.c_str(), &st) == -1)
		throw RespException(404);

	int isDir = S_ISDIR(st.st_mode);
	if (isDir)
	{
		if (filePath_[filePath_.size() -1] != '/')
			filePath_ += '/';
		if (location_->index.empty())
            throw RespException(404);
		filePath_ += location_->index;
	}
	openFile_(O_RDONLY, 404);
	if (isDir)
		fstat(fd_, &st);
	payLoadSize_ = st.st_size;
}



void RespGet::writeHeaders_()
{
	initHeaders();
	writeContentType_(filePath_);
	writeContentLength_(payLoadSize_);
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

