#pragma once

#include "Response.hpp"
#include <dirent.h>

class RespDelete: public Response
{
	private:
	int del_it_();
	int delDir_(char*);
	int reachResource_();
	void makeResponse_();
	
	public:
	RespDelete(const Request &r, char[], unsigned int);
	~RespDelete();
	virtual int readResponse();
};
