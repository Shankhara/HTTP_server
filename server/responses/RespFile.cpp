#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
    payload_ = req_.getBody();
    payloadCursor_ = 0;

	addFilePathRoot_();
    negotiateAcceptLang_ ();
    negotiateContentLang_();
}

RespFile::~RespFile() {
	if (fd_ > 0)
		close(fd_);
}

void RespFile::addFilePathRoot_()
{
	if (!req_.getLocation()->root.empty())
		filePath_ = req_.getLocation()->root + req_.getReqTarget();
	else
		filePath_ = req_.getServer()->root + req_.getReqTarget();
}

void RespFile::openFile_(int flags, int exceptionCode)
{
    if (contentLangNegotiated_)
    {
        for (size_t i = 0; i < langFilePath_.size() ; ++i)
        {
            fds_.push_back(open(langFilePath_[i].c_str(), flags, 0664));
            if (fds_[i] == -1)
                throw RespException(exceptionCode);
        }
    }
    else
    {
        fd_ = open(filePath_.c_str(), flags, 0664);
        if (fd_ == -1)
            throw RespException(exceptionCode);
    }
}

void RespFile::write_()
{
    size_t len = payload_.size() - payloadCursor_;
    if (len < 1)
        return;
    int nbytes = write(fd_, payload_.c_str() + payloadCursor_, len);
    if (nbytes == 0)
    {
        Log::get(logERROR) << __FUNCTION__ << " undefined state" << std::endl;
        statusCode_ = 500;
    }
    else if (nbytes == -1)
    {
        Log::get(logERROR) << __FUNCTION__  << " unable to open: " << strerror(errno) << std::endl;
        statusCode_ = 500;
    }
    payloadCursor_ += len;
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
		ret = mkdir(name.c_str(), 0777); 
		if (ret == -1 && errno != EEXIST)
			return -1;
	}
	return 0;
}

void RespFile::negotiateAcceptLang_()
{
    acceptLangNegotiated_ = false;
    if (!req_.getHeaderAcceptLanguage().empty())
    {
        std::string tmp;
        size_t pos = filePath_.rfind('/'); // garantie de toujours trouver un "/" ?
        for(size_t i = 0; i < req_.getHeaderAcceptLanguage().size(); ++i)
        {
            if (req_.getHeaderAcceptLanguage()[i].find('*') != std::string::npos)
                return;
            tmp = filePath_;
            tmp.insert(pos + 1, req_.getHeaderAcceptLanguage()[i] + "/");
            struct stat st;
            int ret = stat(tmp.c_str(), &st);
            if (ret != -1)
            {
                filePath_ = tmp;
                acceptLangNegotiated_ = true;
                return;
            }
        }
        return;
    }
}

void RespFile::negotiateContentLang_()
{
    contentLangNegotiated_ = false;
    if (!req_.getHeaderContentLanguage().empty())
    {
        std::vector<std::string> vector = req_.getHeaderContentLanguage();
        std::string tmp;
        size_t pos = filePath_.rfind('/');
        for(size_t i = 0; i < vector.size(); ++i)
        {
            tmp = filePath_;
            std::transform(vector[i].begin(), vector[i].end(), vector[i].begin(), ft_toupper);
            if (pos != std::string::npos)
                tmp.insert(pos + 1, vector[i] + "/");
            else
                tmp.insert(0,"/" + vector[i] + "/");
            langFilePath_.push_back(tmp);
            createDirectories_(tmp);
        }
        contentLangNegotiated_ = true;
    }
}
