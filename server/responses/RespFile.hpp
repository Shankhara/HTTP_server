#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "RespException.hpp"

class RespFile : public Response
{
	protected:
	virtual void setFilePath_();
	int fd_;
	size_t fileSize_;
	int payloadCursor_;
	bool acceptLangNegotiated_;
	std::string payload_;
	std::string filePath_;
protected:
	std::vector<std::string> langFilePath_;
    int createDirectories_ (const std::string & str);
	void openFile_(int exceptionStatus, int flags);
	int	read_();
	void prefixFilePathWithAcceptLang_();

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();

};
