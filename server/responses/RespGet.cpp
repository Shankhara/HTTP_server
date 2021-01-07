#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): RespFile(r, buf, bufSize)
{
	fd_ = 0;
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();
}

RespGet::~RespGet()
{
	if (fd_ > 0)
		close(fd_);
}

void RespGet::openFile_()
{
	struct stat st;

	if (stat(filePath_.c_str(), &st) == -1)
	{
		statusCode_ = 404;
		return;
	}

	int isDir = S_ISDIR(st.st_mode);
	if (isDir)
	{
		if (filePath_[filePath_.size() -1] != '/')
			filePath_ += '/';
		filePath_ += location_->index;
	}
	Log::get(logDEBUG) << __FUNCTION__  << " PATH: " << filePath_ << " IS_DIR " << isDir \
	<< " INDEX " << location_->index << std::endl;

	fd_ = open(filePath_.c_str(), O_RDONLY);
	if (fd_ == -1)
	{
		Log::get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
		statusCode_ = 500;
		return;
	}
//	if (isDir)
	fstat(fd_, &st);
	payLoadSize_ = st.st_size;
}

int RespGet::readFile_()
{
	int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));
	Log::get(logDEBUG) << __FUNCTION__  << " currentRead " << currentRead << " NBYTES_ " \
	<< nbytes_ << " BUFSIZE_ " << bufSize_ << std::endl;

	if (currentRead < 0)
	{
		Log::get(logERROR) << __FUNCTION__  << " read error " << strerror(errno) << std::endl;
		return nbytes_;
	}
	if (currentRead == 0)
		return currentRead;

	return nbytes_ + currentRead;
}

void RespGet::makeResponse_()
{
	writeStatusLine_(statusCode_);
	if (statusCode_ != 200)
		writeErrorBody(statusCode_);
	else
	{
		writeContentType_(filePath_);
		writeContentLength_(payLoadSize_);
		writeHeadersEnd_();
	}
}

int RespGet::readResponse()
{
	nbytes_ = 0;

	if (statusCode_ != 200)
		return nbytes_;

	if (headersBuilt_ == false)
	{
		if (location_->autoindex && reqTarget_[reqTarget_.size() - 1] == '/')
			return (writeAutoIndex_(location_->root + reqTarget_));

		openFile_();
		makeResponse_();
	}
	return readFile_();
}
