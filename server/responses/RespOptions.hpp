#pragma once

#include "Response.hpp"

class RespOptions: public Response
{
	private:
	void makeResponse_();
	
	public:
	RespOptions(const Request &r, char[], unsigned int);
	~RespOptions();
	virtual int readResponse();
	void build();
};
