#include "RespPost.hpp"

RespPost::RespPost(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize)
{
	fd_ = 0;
	payloadCursor_ = 0;
}

RespPost::~RespPost()
{
	if (fd_ > 0)
		close(fd_);
}

void RespPost::openFile_()
{
	struct stat	buffer;

	int ret = stat(filePath_.c_str(), &buffer);
	if (ret == -1) {
		statusCode_ = 201;
		fd_ = open(filePath_.c_str(), O_CREAT | O_WRONLY, 0664);
	}
	else
		fd_ = open(filePath_.c_str(), O_APPEND | O_WRONLY, 0664);
	if (fd_ == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
		statusCode_ = 500;
		return;
	}
}

void RespPost::postPayload_()
{

	int len = payload_.size();
	if (len == 0)
		return ;
	int nbytes = write(fd_, payload_.c_str(), len);
	if (nbytes == 0) {
		Log::get(logERROR) << __FUNCTION__ << " undefined state" << std::endl;
		statusCode_ = 500;
	}
	else if (nbytes == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
		statusCode_ = 500;
	}
	payloadCursor_ += len;
}

void RespPost::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		writeContentLength_(0);
		writeThisHeader_("Content-type", Mime::getInstance()->getContentType(filePath_));
		writeThisHeader_("Content-location", filePath_);
		writeThisHeader_("Last-Modified", getStrDate());
		writeHeadersEnd_();
	}
}

int RespPost::readResponse()
{
	nbytes_ = 0;

	if (fd_ == 0)
		openFile_();
	if (statusCode_ == 500)
		return writeErrorPage(500);
	payload_ = req_.getBody();
	if (payload_.size() > payloadCursor_)
		postPayload_();
	if (statusCode_ == 500)
		return writeErrorPage(500);
	if (req_.getStatusCode() == 200)
		makeResponse_();
	return nbytes_;
}
