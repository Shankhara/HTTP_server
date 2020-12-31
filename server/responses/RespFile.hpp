#pragma once

#include "Response.hpp"

class RespFile : public Response
{
	private:
	void setFilePath_();

	protected:
	std::string filePath_;

	public:
	RespFile(const Request &r, char[], unsigned int);
	~RespFile();
};
