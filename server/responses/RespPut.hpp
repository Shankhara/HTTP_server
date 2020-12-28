#pragma once

#include "Response.hpp"

class RespPut: public Response
{
	private:
	Parsing::location *location_;
	std::string reqTarget_;
	std::string path_;
	std::string payload_;
	int fd_;
	int fileExists_;
	
	public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();

};
