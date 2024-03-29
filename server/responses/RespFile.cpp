#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize), fd_(0)
{
    payloadCursor_ = 0;
    fileSize_ = 0;
    acceptLanguages_ = req_.getHeaderAcceptLanguage();
}

RespFile::~RespFile() {
    if (fd_ > 0)
        close(fd_);
}

void RespFile::setFilePath_()
{
    if (req_.getLocation() == 0)
        throw RespException(500);
    if (!req_.getLocation()->root.empty())
        filePath_ = req_.getLocation()->root;
    else
        filePath_ = req_.getServer()->root;
    filePath_.append(req_.getReqTarget(), 1);
    prefixFilePathWithAcceptLang_();

    struct stat st;
    int ret = stat(filePath_.c_str(), &st);
    if (ret == -1)
    {
        if (req_.getMethod() == "OPTIONS" || req_.getMethod() == "GET" || req_.getMethod() == "HEAD")
            throw RespException(404);
        return ;
    }
    int isDir = S_ISDIR(st.st_mode);
    if (isDir)
    {
        if (req_.getReqTarget()[req_.getReqTarget().size() - 1] != '/')
            throw RespException(301);
        else if (req_.getMethod() == "DELETE")
            return ;
        else if (!req_.getLocation()->autoindex && req_.getMethod() == "GET")
            throw RespException(404);
        else if (!req_.getLocation()->autoindex && req_.getMethod() == "HEAD")
            throw RespException(404);
    }
    fileSize_ = st.st_size;
}

void RespFile::openFile_(int flags, int exceptionCode)
{
    fd_ = open(filePath_.c_str(), flags, 0664);
    if (fd_ == -1)
        throw RespException(exceptionCode);
}

int RespFile::createDirectories_(const std::string & str)
{
    std::vector<std::string> dirs = explode(str, '/');
    std::string name;
    int ret;

    dirs.pop_back();
    for (size_t i = 0; i < dirs.size(); i++)
    {
        name.append("/");
        name.append(dirs[i]);
        ret = mkdir(name.c_str(), 0755);
        if (ret == -1 && errno != EEXIST)
            return -1;
    }
    return 0;
}

int RespFile::read_()
{
    int currentRead = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));

    if (currentRead < 0)
    {
        Log::get(logERROR) << __FUNCTION__  << " read error " << strerror(errno) << std::endl;
        return currentRead;
    }
    return nbytes_ + currentRead;
}

void RespFile::prefixFilePathWithAcceptLang_()
{
    acceptLangNegotiated_ = false;
    if (!req_.getHeaderAcceptLanguage().empty())
    {
        std::string tmp;
        struct stat st;
        size_t pos = filePath_.rfind('/');
        while (!acceptLanguages_.empty())
        {
            tmp = filePath_;
            tmp.insert(pos + 1, acceptLanguages_[0] + "/");
            int ret = stat(tmp.c_str(), &st);
            if (ret != -1)
            {
                filePath_ = tmp;
                acceptLangNegotiated_ = true;
                return;
            }
            acceptLanguages_.erase(acceptLanguages_.begin());
        }
        return;
    }
}

