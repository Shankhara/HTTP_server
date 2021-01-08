#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	addFilePathRoot_();
    langNegotiation_();
}

RespFile::~RespFile() { }

void RespFile::addFilePathRoot_()
{
	if (!req_.getLocation()->root.empty())
		filePath_ = req_.getLocation()->root + req_.getReqTarget();
	else
		filePath_ = req_.getServer()->root + req_.getReqTarget();
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

void RespFile::langNegotiation_()
{
    langNegotiated_ = false;
    if (!req_.getHeaderAcceptLanguage().empty())
    {
        std::string tmp = filePath_;
        for(size_t i = 0; i < req_.getHeaderAcceptLanguage().size(); ++i)
        {
            size_t pos = tmp.rfind('/'); // garantie de toujours trouver un "/" ?
            tmp.insert(++pos, req_.getHeaderAcceptLanguage()[i] + "/");
            struct stat st;
            int ret = stat(tmp.c_str(), &st);
            if (ret != -1)
            {
                filePath_ = tmp;
                langNegotiated_ = true;
            }
        }
        return;
    }
}
