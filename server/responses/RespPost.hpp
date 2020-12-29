#pragma once

#include "Response.hpp"

class RespPost: public Response
{
	private:
	void postPayload_();
	void makeResponse_();
	
	public:
	RespPost(const Request &r, char[], unsigned int);
	~RespPost();
	virtual int readResponse();
};
