#pragma once

#include "RespFile.hpp"

class RespPost: public RespFile
{
	private:
	int			fd_;
	std::string payload_;
	unsigned int payloadCursor_;

	void postPayload_();
	void makeResponse_();
	void manageFile_();
	
	public:
	RespPost(const Request &r, char[], unsigned int);
	~RespPost();
	virtual int readResponse();
	void build();
};
