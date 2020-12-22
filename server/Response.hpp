#pragma once

#include "Request.hpp"

class Response
{
	private:
	Request req_;
	std::string msg_;
	int statusCode_;

	std::map<int, std::string> statusMap_;
	std::vector<std::string> headersToPut_;

	public:
	Response(const Request &);
	~Response();

	void createResponse();
	void setHeaders();
	void putHeaders();

	std::string getResponseMsg() const;

};
