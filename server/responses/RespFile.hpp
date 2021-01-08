#pragma once

#include "Response.hpp"
#include <sys/stat.h>
#include <errno.h>
#include "RespException.hpp"

class RespFile : public Response
{
	private:
	void addFilePathRoot_();
    void langNegotiation_();

	protected:
	std::string filePath_;
	bool langNegotiated_;
	int createDirectories_();

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();

};
