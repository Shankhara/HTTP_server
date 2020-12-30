#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	setFilePath();
}

RespPut::~RespPut() { }

bool RespPut::reachResource_()
{
	if (filePath_[filePath_.size() - 1] == '/')
		return false;

	struct stat buffer;
	if (stat(filePath_.c_str(), &buffer) == -1)
		statusCode_ = 201;
	else
		statusCode_ = 200;

	fd_ = open(filePath_.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0664);
	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
		return (false);
	}
	return (true);
}

int RespPut::compareFiles_()
{
	char buff[255];
	size_t nbytes; 
	std::string str;

	while ((nbytes = read(fd_, buff, 254)) > 0)
	{
		buff[nbytes] = '\0';
		str.append(buff);
	}
	return (str.compare(payload_) == 0);
}

void RespPut::putPayload_()
{
	int len = payload_.size();
	int nbytes = write(fd_, payload_.c_str(), len);
	
	if (nbytes != len)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
	}
}

void RespPut::makeResponse_()
{
	if (headersBuilt_ == false)
	{
		writeStatusLine_(statusCode_);
		writeThisHeader_("Content-type", Mime::getInstance()->getContentType(filePath_));
		writeThisHeader_("Content-location", filePath_);
		if (!compareFiles_())
			writeThisHeader_("Last-Modified", getStrDate());
		writeHeadersEnd_();
	}
}

int RespPut::readResponse()
{
	nbytes_ = 0;
	if (fd_ == 0)
	{
		if (reachResource_())
			putPayload_();
		makeResponse_();
	}
	return nbytes_;
}
