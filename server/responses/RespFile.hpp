#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "RespException.hpp"

class RespFile : public Response
{
	private:
	void addFilePathRoot_();
    void negotiateAcceptLang_();
    void negotiateContentLang_ ();

	protected:
	int fd_;
	int payloadCursor_;
	std::string payload_;
	std::vector<int> fds_;
	std::string filePath_;
	bool acceptLangNegotiated_;
    bool contentLangNegotiated_;
    std::vector<std::string> langFilePath_;
    int createDirectories_ (const std::string & str);
	void openFile_(int exceptionStatus, int flags);

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();

void write_ ();
};
