#include "RespPost.hpp"

RespPost::RespPost(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize)
{
	fd_ = 0;
	payload_ = req_.getBody();
}

RespPost::~RespPost()
{
	if (fd_ > 0)
		close(fd_);
}

void RespPost::postPayload_()
{
	struct stat	buffer;

	int ret = stat(filePath_.c_str(), &buffer);
	if (ret == -1)
		fd_ = open(filePath_.c_str(), O_CREAT | O_WRONLY, 0664);
	else
		fd_ = open(filePath_.c_str(), O_APPEND | O_WRONLY, 0664);

	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
		return;
	}

	int len = payload_.size();
	int nbytes = write(fd_, payload_.c_str(), len);
	if (nbytes == len)
	{
		if (ret == -1)
			statusCode_ = 201;
		else
			statusCode_ = 200;
	}
	else
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
	}
}

void RespPost::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		if (statusCode_ != 500)
		{
			writeThisHeader_("Content-type", Mime::getInstance()->getContentType(filePath_));
			writeThisHeader_("Content-location", filePath_);
			writeThisHeader_("Last-Modified", getStrDate());
			writeHeadersEnd_();
		}
		else
		{
			writeHeadersEnd_();
			writeErrorBody(statusCode_);
		}
	}
}

int RespPost::readResponse()
{
	nbytes_ = 0;

	postPayload_();
	makeResponse_();

	return nbytes_;
}
