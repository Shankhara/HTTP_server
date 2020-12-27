#pragma once

#include "Response.hpp"

#define BUFFER_SIZE 32768

class RespPut: public Response
{
	private:
	Parsing::location *location_;
	std::string reqTarget_;
	std::string path_;
	int fd_;
	int statusCode_;
	int fileExists_;
	
	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();

};
