#include "RespFile.hpp"

RespFile::RespFile(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize), fd_(0)
{
	payloadCursor_ = 0;
}

RespFile::~RespFile() {
	if (fd_ > 0)
		close(fd_);
}

void RespFile::setFilePath_()
{
	if (req_.getLocation() == 0){
		filePath_ = "";
		return ;
	}
	if (!req_.getLocation()->root.empty())
		filePath_ = req_.getLocation()->root + req_.getReqTarget();
	else
		filePath_ = req_.getServer()->root + req_.getReqTarget();
	prefixFilePathWithAcceptLang_();
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
		return nbytes_;
	}
	return nbytes_ + currentRead;
}

void RespFile::prefixFilePathWithAcceptLang_()
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
