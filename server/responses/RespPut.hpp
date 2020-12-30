#pragma once

#include "Response.hpp"

class RespPut: public Response
{
	private:
	std::string 		path_;
	std::string 		payload_;
	int 				fd_;
	size_t				payloadCursor_;
	int					fileExists_;
	int 				statusCode_;

	void putPayload_();
	bool reachResource_();
	void makeResponse_();
	int compareFiles_();

	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
	virtual int readResponse();
};
