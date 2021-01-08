#pragma once

#include "RespFile.hpp"

class RespPut: public RespFile
{
	private:
	void reachResource_();
	void makeResponse_();

	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
	virtual void build();
};
