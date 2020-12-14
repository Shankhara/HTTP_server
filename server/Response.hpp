#pragma once

#include "Request.hpp"

class Response
{
	private:
	Request request_;
	std::string msg_;

	public:
	Response(const Request &);
	~Response();

	void createResponse();
	void statusLine();
	void statusCode();

	std::string getResponseMsg() const;

};
