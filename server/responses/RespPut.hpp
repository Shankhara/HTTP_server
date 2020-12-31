#pragma once

#include "RespFile.hpp"

class RespPut: public RespFile
{
	private:
	int fd_;
	size_t payloadCursor_;

	void putPayload_();
	bool reachResource_();
	void makeResponse_();
	int compareFiles_();

	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
};
