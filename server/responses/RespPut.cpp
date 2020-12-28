#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	fd_ = 0;
	statusCode_ = 200;
	fileExists_ = false;
	payload_ = req_.getBody();

	setPath();

	if (reachResource());
		putPayload();
}

RespPut::~RespPut() { }

void ResPut::setPath()
{
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();

	if (!location->root.empty())
		path_ = location->root + reqTarget();
	else
		path_ = req_.getServer()->root + reqTarget(); 

//	struct stat st;
//	stat(path_.c_str(), &st);
}

bool reachResource()
{
	struct stat buffer;
	if (!stat(path_, &buffer))
		fileExists_ = true;

	fd_ = open(path_, O_CREAT, 0664);
	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
		return (false);
	}
	return (true);
}

void RespPut::putPayload()
{
	static size_t len = payload_.size();

	if ((nbytes = write(fd_, payload_, len)) != len)
		statusCode_ = 500;
	else if (fileExists_)
		statusCode_ = 200;
	else
		statusCode_ = 201;
}

int RespPut::makeResponse()
{
	append_
}

int RespPut::readResponse()
{

}
