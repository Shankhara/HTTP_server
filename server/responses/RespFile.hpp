#pragma once

#include "Response.hpp"
#include <sys/stat.h>

class RespFile : public Response
{
	private:
	void setFilePath_();
	void createDirectories_();

	protected:
	std::string filePath_;

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();
};
