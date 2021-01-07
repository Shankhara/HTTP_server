#pragma once

#include "Response.hpp"

class RespTrace: public Response
{
	private:
	void writeHeaders_();
	std::string request_;
	
	public:
	RespTrace(const Request &r, char[], unsigned int);
	~RespTrace();
	virtual int readResponse();
	void build();
};
