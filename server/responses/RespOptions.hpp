#pragma once

#include "RespFiles.hpp"

class RespOptions: public RespFiles
{
	private:
	void makeResponse_();

	public:
	RespOptions(const Request &r, char[], unsigned int);
	~RespOptions();
	virtual int readResponse();
	void build();
};
