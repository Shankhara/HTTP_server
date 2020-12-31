#pragma once

#include "RespFile.hpp"
#include <dirent.h>

class RespDelete: public RespFile
{
	private:
	int delDir_(std::string &);
	int delResource_();
	void makeResponse_();
	
	public:
	RespDelete(const Request &r, char[], unsigned int);
	~RespDelete();
	virtual int readResponse();
};
