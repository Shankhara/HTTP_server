#include "RespPut.hpp"

RespPut::RespPut(const Request &r, char buf[], unsigned int bufSize) : RespFiles(r, buf, bufSize)
{ }

RespPut::~RespPut(){}

void RespPut::makeResponse_()
{
	initHeaders();
	writeContentType_(filePath_);
	writeContentLength_(0);
    if (contentLangNegotiated_)
    {
        for(size_t i = 0; i < langFilePath_.size(); ++i)
            writeThisHeader_ ("Content-location", langFilePath_[i]);
    }
    else if (statusCode_ == 201)
		writeThisHeader_("Location", filePath_);
	writeThisHeader_("Last-Modified", getStrDate());
	writeHeadersEnd_();
}

int RespPut::readResponse()
{
	nbytes_ = 0;

	writeFiles_();
	if (statusCode_ == 500)
		return -1;
	if (req_.getStatusCode() == 200 && !headersBuilt_)
		makeResponse_();
	return nbytes_;
}

void RespPut::build() {
	setFilePath_();
	negotiateContentLang_();
    reachResource_();
}