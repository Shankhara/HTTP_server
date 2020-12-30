#pragma once

#include "Response.hpp"
#include <dirent.h>

class RespDelete: public Response
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
