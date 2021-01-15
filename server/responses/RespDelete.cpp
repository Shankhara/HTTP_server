#include "RespDelete.hpp"

RespDelete::RespDelete(const Request &r, char buf[], unsigned int bufSize) : RespFiles(r, buf, bufSize) { }

RespDelete::~RespDelete() { }

int RespDelete::delDir_(std::string & param)
{
    const char *c_param = param.c_str();
    DIR *dir = opendir(c_param);
    if (!dir)
    {
        Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
        throw RespException(500);
    }
    struct dirent *entry;
    std::string name, concat_path;
    while ((entry = readdir(dir)))
    {
        name = entry->d_name;
        if (name == "." || name == "..")
            continue;
        concat_path += param;
        concat_path += "/";
        concat_path += name;
        if (entry->d_type == DT_DIR)
            delDir_(concat_path);
        else
            unlink(concat_path.c_str());
    }
    if (rmdir(c_param) == -1)
    {
        Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
        closedir(dir);
        throw RespException(500);
    }
    closedir(dir);
    return EXIT_SUCCESS;
}

int RespDelete::delResource_()
{
    int ret;
    struct stat statbuf = {};
    if (contentLangNegotiated_)
    {
        for(size_t i = 0; i < langFilePath_.size(); ++i)
        {
            ret = stat(langFilePath_[i].c_str(), &statbuf);
            if (S_ISDIR(statbuf.st_mode))
                delDir_(langFilePath_[i]);
            else
                unlink(langFilePath_[i].c_str());
        }
        if (!ret)
            return 0;
    }
    else
        ret = stat(filePath_.c_str(), &statbuf);
    if (ret == -1)
    {
        Log::get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
        throw RespException(404);
    }
    if (!S_ISDIR(statbuf.st_mode))
        return unlink(filePath_.c_str());
    return delDir_(filePath_);
}

void RespDelete::makeResponse_()
{
    if (!headersBuilt_)
    {
        initHeaders();
        writeHeadersEnd_ ();
    }
}

int RespDelete::readResponse()
{
    nbytes_ = 0;
    if (!headersBuilt_)
        makeResponse_();
    return nbytes_;
}

void RespDelete::build()
{
    setFilePath_();
    negotiateContentLang_();
    if (!delResource_())
        statusCode_ = 204;
}
