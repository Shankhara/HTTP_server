#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): RespFile(r, buf, bufSize)
{
	fd_ = 0;
	location_ = req_.getLocation();
	reqTarget_ = req_.getReqTarget();
}

RespGet::~RespGet(){}

void RespGet::reachRessource_()
{
	struct stat st;

	if (stat(filePath_.c_str(), &st) == -1)
		throw RespException(404);

	int isDir = S_ISDIR(st.st_mode);
	if (isDir)
	{
		if (filePath_[filePath_.size() -1] != '/')
			filePath_ += '/';
		filePath_ += location_->index;
	}
	openFile_(O_RDONLY, 404);
	if (isDir)
		fstat(fd_, &st);
	payLoadSize_ = st.st_size;
}

int RespGet::readFile_()
{
	int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));

	if (currentRead < 0)
	{
		Log::get(logERROR) << __FUNCTION__  << " read error " << strerror(errno) << std::endl;
		return nbytes_;
	}
	return nbytes_ + currentRead;
}

void RespGet::writeHeaders_()
{
	writeFirstPart_();
	writeContentType_(filePath_);
	writeContentLength_(payLoadSize_);
    if(langNegotiated_)
        writeThisHeader_("Content-Location", filePath_);
	writeHeadersEnd_();
}

void RespGet::build()
{
	if (location_->autoindex == false || reqTarget_[reqTarget_.size() - 1] != '/')
		reachRessource_();
}

int RespGet::readResponse()
{
	nbytes_ = 0;

	if (headersBuilt_ == false)
	{
		if (location_->autoindex && reqTarget_[reqTarget_.size() - 1] == '/')
			return (writeAutoIndex_(location_->root + reqTarget_));
		writeHeaders_();
	}
	return readFile_();
}

