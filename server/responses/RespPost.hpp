#pragma once

#include "RespFile.hpp"

class RespPost: public RespFile
{
	private:
	void makeResponse_();
	void manageFile_();
	
	public:
	RespPost(const Request &r, char[], unsigned int);
	~RespPost();
	virtual int readResponse();
	void build();
};
