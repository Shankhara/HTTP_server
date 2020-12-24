#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): Response(r, buf, bufSize) {
	fd_ = 0;
}

RespGet::~RespGet() {
	if (fd_ > 0)
		close(fd_);
}

int RespGet::readResponse() {
	if (fd_ == -1)
		return 0;
	if (headersBuilt_ == false)
	{
		openFile_();
		if (fd_ == -1)
			return (writeErrorPage(404));

	}
	return(readFile_());
}

int RespGet::readFile_() {
	int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));
	Log().Get(logDEBUG) << __FUNCTION__  << " currentRead " << currentRead << " NBYTES_ " << nbytes_ << " BUFSIZE " << bufSize_;
	if (currentRead < 0)
		Log().Get(logERROR) << __FUNCTION__  << " read error " << strerror(errno);
	currentRead += nbytes_;
	nbytes_ = 0;
	return (currentRead);
}

void RespGet::openFile_() {
	Parsing::location *location = req_.getLocation();
	std::string path = location->root + req_.getReqTarget();
	Log().Get(logDEBUG) << __FUNCTION__  << " PATH: " << path;

	fd_ = open(path.c_str(), O_RDONLY);
	if (fd_ == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		return ;
	}
	struct stat st;
	fstat(fd_, &st);
	Log().Get(logDEBUG) << "STATUS: " << nbytes_;
	appendStatusCode(200);
	appendBaseHeaders();
	setHeaderContentLength(st.st_size);
	setHeaderContentType(std::string("text/html")); //TODO
	appendHeadersEnd();
	headersBuilt_ = true;
}
