#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	addFilePathRoot_();
	negotiateLangAccepted_();
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
	fd_ = open(filePath_.c_str(), flags, 0664);
	if (fd_ == -1)
		throw RespException(exceptionCode);
}

int RespFile::createDirectories_()
{
	std::vector<std::string> dirs = explode(filePath_, '/');
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

void RespFile::negotiateLangAccepted_()
{
    langNegotiated_ = false;
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
                langNegotiated_ = true;
                return;
            }
        }
        return;
    }
}

void RespFile::contentLangNegotiation()
{
    if (!req_.getHeaderContentLanguage().empty())
    {
        std::vector<std::string> vector = req_.getHeaderContentLanguage();
        size_t pos = filePath_.rfind('/');
        for(size_t i = 0; i < vector.size(); ++i)
        {
            std::transform(vector[i].begin(), vector[i].end(), vector[i].begin(), ft_toupper);
            if (pos != std::string::npos)
                filePath_.insert(pos + 1, vector[i] + "/");
            else
                filePath_.insert(0,"/" + vector[i] + "/");
            createDirectories_();
        }

    }
}
