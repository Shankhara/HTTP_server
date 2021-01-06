#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	setFilePath_();
}

RespFile::~RespFile() {
}

void RespFile::setFilePath_()
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
