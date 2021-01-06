#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>

class RespFile : public Response
{
	private:
	void setFilePath_();

	protected:
	std::string filePath_;
	int createDirectories_();

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();
};
