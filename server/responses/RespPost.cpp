#include "RespPost.hpp"

RespPost::RespPost(const Request &r, char buf[], unsigned int bufSize) : RespFiles(r, buf, bufSize)
{
	fd_ = 0;
	payloadCursor_ = 0;
}

RespPost::~RespPost() { }

void RespPost::manageFile_()
{
	struct stat	st;

	if (createDirectories_(filePath_) == -1)
		throw RespException(500);

	int ret = stat(filePath_.c_str(), &st);
	if (ret == -1)
		statusCode_ = 201;

	openFiles_(O_CREAT | O_WRONLY, 500);
}

void RespPost::makeResponse_()
{
	initHeaders();
	writeContentType_(filePath_);
	writeContentLength_(0);
	if (contentLangNegotiated_)
    {
        for(size_t i = 0; i < langFilePath_.size(); ++i)
            writeThisHeader_ ("Content-location", langFilePath_[i]);
    }
	else
    	writeThisHeader_ ("Content-location", filePath_);
	writeThisHeader_ ("Last-Modified", getStrDate ());
	writeHeadersEnd_ ();
}

int RespPost::readResponse()
{
	nbytes_ = 0;

	writeFiles_();
	if (statusCode_ == 500)
		return -1;
	if (!headersBuilt_ && req_.getStatusCode() == 200)
		makeResponse_();
	return nbytes_;
}

void RespPost::build()
{
	setFilePath_();
	negotiateContentLang_();
	manageFile_();
}