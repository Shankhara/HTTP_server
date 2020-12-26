#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): Response(r, buf, bufSize) {
	fd_ = 0;
}

RespGet::~RespGet() {
	if (fd_ > 0)
		close(fd_);
}

int RespGet::readResponse() {
	nbytes_ = 0;
	if (fd_ == -1)
		return 0;
	if (fd_ == 0)
	{
		fd_ = -1;
		Parsing::location *location = req_.getLocation();
		if (req_.getReqTarget()[req_.getReqTarget().size() - 1] == '/')
			return (writeAutoIndex_(location->root + req_.getReqTarget()));
		openFile_(location);
		if (fd_ == -1)
			return (writeErrorPage(404));
	}
	return(readFile_());
}

int RespGet::readFile_() {
	int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));
	Log().Get(logDEBUG) << __FUNCTION__  << " currentRead " << currentRead << " NBYTES_ " << nbytes_ << " BUFSIZE_ " << bufSize_;
	if (currentRead < 0) {
		Log().Get(logERROR) << __FUNCTION__  << " read error " << strerror(errno);
		return (nbytes_);
	}
	return (currentRead + nbytes_);
}

void RespGet::openFile_(Parsing::location *location) {
	std::string path = location->root + req_.getReqTarget();
	bool isDir;

	Log().Get(logDEBUG) << __FUNCTION__  << " PATH: " << path;
	struct stat st;
	stat(path.c_str(), &st);
	isDir = S_ISDIR(st.st_mode);
	if (isDir)
	{
		if (location->index.size() == 0) //TODO: conf should have default settings available in every location #33
			path += "index.html";
		else
			path += location->index;
	}
	fd_ = open(path.c_str(), O_RDONLY);
	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		return ;
	}
	if (isDir)
		fstat(fd_, &st);
	appendHeaders("text/html", st.st_size);
	headersBuilt_ = true;
}
