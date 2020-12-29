#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	fd_ = 0;
	payloadCursor_ = 0;
	statusCode_ = 200;
	fileExists_ = false;
	payload_ = req_.getBody();

	setPath_();
}

RespPut::~RespPut() { }

void RespPut::setPath_()
{
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();

	if (!location_->root.empty())
		path_ = location_->root + reqTarget_;
	else
		path_ = req_.getServer()->root + reqTarget_; 
}

bool RespPut::reachResource_()
{
	if (path_[path_.size() - 1] == '/')
		return false;

	struct stat buffer;
	if (!stat(path_.c_str(), &buffer))
		fileExists_ = true;

	fd_ = open(path_.c_str(), O_CREAT|O_RDWR, 0664);
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
	int nbytes;

	std::string payload = req_.getBody();
	size_t len = payload.size() - payloadCursor_;
	nbytes = write(fd_, payload.c_str() + payloadCursor_, len);
	payloadCursor_ += len;
	if (nbytes < 0)
		statusCode_ = 500;
	else if (fileExists_)
		statusCode_ = 200;
	else
		statusCode_ = 201;
}

void RespPut::makeResponse_()
{
	writeStatusLine_(statusCode_);
	//writeThisHeader_("Content-type", Mime::getInstance()->getContentType(path_));
	writeContentType_("text/html"); // dans mon tcpdump le contentType etait vide
	writeThisHeader_("Content-location", path_);
	if (!compareFiles_())
		writeThisHeader_("Last-Modified", getStrDate());
	writeHeadersEnd_();
	headersBuilt_ = true;
}

int RespPut::readResponse()
{
	nbytes_ = 0;

	if (fd_ > 0)
	{
		if (headersBuilt_)
			return 0;
		putPayload_();
		if (req_.getStatusCode() == 200)
			makeResponse_();
		else return -1;
	}
	if (fd_ == 0)
	{
		if (reachResource_())
			putPayload_();
		if (req_.getStatusCode() == 200)
			makeResponse_();
		else return -1;

	}
	return nbytes_;
}
