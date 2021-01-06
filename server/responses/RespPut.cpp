#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize)
{
	fd_ = 0;
	payloadCursor_ = 0;
}

RespPut::~RespPut()
{
	if (fd_ > 0)
		close(fd_);
}

bool RespPut::reachResource_()
{
	// TODO: how?
	if (filePath_[filePath_.size() - 1] == '/')
		return false;

	if (createDirectories_() == -1)
	{
		statusCode_ = 500;
		return (false);
	}

	struct stat buffer = {};
	if (stat(filePath_.c_str(), &buffer) == -1)
		statusCode_ = 201;

	fd_ = open(filePath_.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd_ == -1)
	{
		Log::get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
		statusCode_ = 500;
		return (false);
	}
	return (true);
}

void RespPut::putPayload_()
{
	int nbytes;

	std::string payload = req_.getBody();
	size_t len = payload.size() - payloadCursor_;
	nbytes = write(fd_, payload.c_str() + payloadCursor_, len);
	payloadCursor_ += len;
	if (nbytes < 0)
		statusCode_ = 500;
}

void RespPut::makeResponse_()
{
	writeStatusLine_(statusCode_);
	writeThisHeader_("Content-type", Mime::getInstance()->getContentType(filePath_));
	if (statusCode_ == 201)
		writeThisHeader_("Location", filePath_);
	writeThisHeader_("Last-Modified", getStrDate());
	writeHeadersEnd_();
	writeHeadersEnd_();
}

int RespPut::readResponse()
{
	nbytes_ = 0;
	if (fd_ == 0)
		reachResource_();

	if (fd_ > 0)
	{
		putPayload_();
		if (req_.getStatusCode() == 200 && !headersBuilt_)
			makeResponse_();
	}
	if (fd_ == -1)
		writeErrorPage(500);
	Log::get(logDEBUG) << __FUNCTION__ << " NBYTES " << nbytes_ << std::endl;
	return nbytes_;
}

