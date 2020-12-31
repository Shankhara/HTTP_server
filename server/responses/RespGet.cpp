#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): RespFile(r, buf, bufSize)
{
	fd_ = 0;
}

RespGet::~RespGet() {
	if (fd_ > 0)
		close(fd_);
}

int RespGet::readResponse() {
	if (fd_ == -1)
		return 0;
	nbytes_ = 0;
	if (fd_ == 0)
	{
		fd_ = -1;
		Parsing::location *location = req_.getLocation();
		if (req_.getLocation()->autoindex && req_.getReqTarget()[req_.getReqTarget().size() - 1] == '/')
			return (writeAutoIndex_(location->root + req_.getReqTarget()));
		openFile_(location);
		if (fd_ == -1)
			return (writeErrorPage(404));
	}
	return(readFile_());
}

int RespGet::readFile_() {
	int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));
	Log::get(logDEBUG) << __FUNCTION__  << " currentRead " << currentRead << " NBYTES_ " << nbytes_ << " BUFSIZE_ " << bufSize_;
	if (currentRead < 0) {
		Log::get(logERROR) << __FUNCTION__  << " read error " << strerror(errno);
		return (nbytes_);
	}
	return (currentRead + nbytes_);
}

void RespGet::openFile_(Parsing::location *location)
{
	int isDir;
	struct stat st;
	stat(filePath_.c_str(), &st);
	isDir = S_ISDIR(st.st_mode);
	if (isDir != 0)
	{
		if (filePath_[filePath_.size() -1] != '/')
			filePath_ += '/';
		filePath_ += location->index;
	}
	Log::get(logDEBUG) << __FUNCTION__  << " PATH: " << filePath_ << " IS_DIR " << isDir << " INDEX " << location->index;
	fd_ = open(filePath_.c_str(), O_RDONLY);
	if (fd_ == -1)
	{
		Log::get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		return ;
	}
	if (isDir)
		fstat(fd_, &st);
	appendHeaders(200, "text/html", st.st_size);
	headersBuilt_ = true;
}
