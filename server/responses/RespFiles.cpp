#include "RespFiles.hpp"

RespFiles::RespFiles(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize)
{
	setFilePath_();
	negotiateContentLang_();
}

RespFiles::~RespFiles() {
	for(size_t i = 0; i < fds_.size(); ++i)
	{
		if (fds_[i] > 0)
			close(fds_[i]);
	}
}

void RespFiles::openFiles_(int flags, int exceptionCode){
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
		fds_.push_back(open(filePath_.c_str(), flags, 0664));
		if (fds_[0] == -1)
			throw RespException(exceptionCode);
	}
}

void RespFiles::writeFiles_()
{
	payload_ = req_.getBody();
	size_t len = payload_.size() - payloadCursor_;
	for (size_t i = 0; i < fds_.size(); i++)
	{
		if (len < 1)
			return;
		int nbytes = write(fds_[i], payload_.c_str() + payloadCursor_, len);
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
	}
	payloadCursor_ += len;
}



void RespFiles::negotiateContentLang_()
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
				tmp.insert(0, "/" + vector[i] + "/");
			langFilePath_.push_back(tmp);
			createDirectories_(tmp);
		}
		contentLangNegotiated_ = true;
	}
}


