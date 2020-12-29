#pragma once

#include "Response.hpp"

class RespPut: public Response
{
	private:
	void putPayload_();
	bool reachResource_();
	void makeResponse_();
	int compareFiles_();
	
	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
};
