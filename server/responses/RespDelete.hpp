#pragma once

#include "Response.hpp"

class RespDelete: public Response
{
	private:
	void del_it_();

	
	public:
	RespDelete(const Request &r, char[], unsigned int);
	~RespDelete();
	virtual int readResponse();
};
