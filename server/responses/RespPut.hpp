#pragma once

#include "RespFiles.hpp"

class RespPut: public RespFiles
{
	private:
	void reachResource_();
	void makeResponse_();
    void reachLangResources_();

	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
	virtual void build();

};
