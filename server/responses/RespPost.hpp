#pragma once

#include "RespFiles.hpp"

class RespPost: public RespFiles
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
