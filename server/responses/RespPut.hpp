#pragma once

#include "RespFile.hpp"

class RespPut: public RespFile
{
	private:
	int fd_;
	size_t payloadCursor_;

	void putPayload_();
	void reachResource_();
	void makeResponse_();

	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
	virtual void build();
};
