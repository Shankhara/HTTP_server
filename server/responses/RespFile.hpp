#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "RespException.hpp"

class RespFile : public Response
{
	private:
	void addFilePathRoot_();
    void negotiateLangAccepted_();

	protected:
	int fd_;
	std::string filePath_;
	bool langNegotiated_;
	int createDirectories_();
	void openFile_(int exceptionStatus, int flags);

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();

void contentLangNegotiation ();
};
