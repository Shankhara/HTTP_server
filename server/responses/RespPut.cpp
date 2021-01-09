#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize)
{ }

RespPut::~RespPut(){}

void RespPut::reachResource_()
{
	if (filePath_[filePath_.size() - 1] == '/')
		throw RespException(400);

	if (createDirectories_(filePath_) == -1)
		throw RespException(500);

	struct stat buffer = {};
	if (stat(filePath_.c_str(), &buffer) == -1)
		statusCode_ = 201;

	openFile_(O_CREAT | O_TRUNC | O_RDWR, 500);
}

void RespPut::makeResponse_()
{
	initHeaders();
	writeContentType_(filePath_);
	if (statusCode_ == 201)
		writeThisHeader_("Location", filePath_);
	writeThisHeader_("Last-Modified", getStrDate());
	writeHeadersEnd_();
	writeHeadersEnd_();
}

int RespPut::readResponse()
{
	nbytes_ = 0;

	write_();
	if (statusCode_ == 500)
		return -1;
	if (req_.getStatusCode() == 200 && !headersBuilt_)
		makeResponse_();
	return nbytes_;
}

void RespPut::build() {
	reachResource_();
}