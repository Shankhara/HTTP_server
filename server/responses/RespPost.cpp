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

void RespPost::manageFile_()
{
	struct stat	st;

	if (createDirectories_() == -1)
		throw RespException(500);

	int ret = stat(filePath_.c_str(), &st);
	if (ret == -1)
		statusCode_ = 201;

	fd_ = open(filePath_.c_str(), O_CREAT | O_WRONLY, 0664);
	if (fd_ == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
		throw RespException(500);
	}
}

void RespPost::postPayload_()
{

	payload_ = req_.getBody();
	int len = payload_.size() - payloadCursor_;
	if (len < 1)
		return ;
	int nbytes = write(fd_, payload_.c_str() + payloadCursor_, len);
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
		writeStatusLine_(statusCode_);
		writeBaseHeaders_();
		writeContentType_(filePath_);
		writeThisHeader_("Content-location", filePath_);
		writeThisHeader_("Last-Modified", getStrDate());
		writeHeadersEnd_();
		writeHeadersEnd_();
}

int RespPost::readResponse()
{
	nbytes_ = 0;

	postPayload_();
	if (statusCode_ == 500)
		return -1;
	if (headersBuilt_ == false && req_.getStatusCode() == 200)
		makeResponse_();
	return nbytes_;
}

void RespPost::build() {
	manageFile_();
}
