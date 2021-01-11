#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "RespException.hpp"

class RespFile : public Response
{
	private:
	void setFilePath_();

	protected:
	int fd_;
	int payloadCursor_;
	bool acceptLangNegotiated_;
	std::string payload_;
	std::string filePath_;
    std::vector<std::string> langFilePath_;
    int createDirectories_ (const std::string & str);
	void openFile_(int exceptionStatus, int flags);
    void write_();
	int	read_();
	void prefixFilePathWithAcceptLang_();

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();

};
