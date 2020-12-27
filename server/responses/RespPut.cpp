#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	setPath();
	fd_ = 0;
	statusCode_ = 200;
	fileExists_ = false;
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

	// est-ce que initialement, le root du location s'ajoute a celui du serveur ?

	struct stat st;
	stat(path.c_str(), &st);

	int isDir = S_ISDIR(st.st_mode);
	if (isDir != 0)
	{
		if (path_[path_.size() -1] != '/')
			path_ += '/';
		path_ += location->index;
	}
}

void reachResource()
{
	struct stat buffer;
	if (!stat(path_, &buffer))
		fileExists_ = true;

	fd_ = open(path_, O_CREAT, 0664);
	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		return;
	}

	size_t len = req_.getBody().size();
	if ((nbytes = write(fd_, req_.getBody(), len)) != len)
		statusCode = 500; // whole file not written
	
	//WIP ...
}

int RespPut::readResponse()
{
	reachResource();
}
