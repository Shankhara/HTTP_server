#include "RespPost.hpp"

RespPost::RespPost(const Request &r, char buf[], unsigned int bufSize) : RespFiles(r, buf, bufSize)
{
    fd_ = 0;
    payloadCursor_ = 0;
}

RespPost::~RespPost() { }

void RespPost::makeResponse_()
{
    initHeaders();
    writeContentType_(filePath_);
    writeContentLength_(0);
    if (contentLangNegotiated_)
    {
        size_t pos = req_.getOriginalReqTarget().rfind('/');
        for(size_t i = 0; i < langFilePath_.size(); ++i)
        {
            std::string tmp = req_.getOriginalReqTarget();
            writeThisHeader_("Content-location", tmp.insert(pos, "/" + req_.getHeaderContentLanguage()[i]));
        }
    }
    else if (statusCode_ == 201)
        writeThisHeader_("Location", req_.getOriginalReqTarget());
    else if (req_.requestIndexed)
        writeThisHeader_("Content-Location", req_.getLocation()->name + req_.getLocation()->index);
    else
        writeThisHeader_ ("Content-location", req_.getOriginalReqTarget());
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
    reachResource_();
    openFiles_(O_CREAT | O_APPEND | O_RDWR, 404);
}