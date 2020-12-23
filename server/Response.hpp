#pragma once

#include "Request.hpp"

class Response
{
	protected:
	Request req_;
	std::string msg_;
	int statusCode_;

	std::map<int, std::string> statusMap_;
	std::vector<std::string> headersToPut_;

	public:
	Response(const Request &);
	virtual ~Response();
	void createResponse();
	void setBaseHeaders();
	void setHeader();
	void putHeaders();
	virtual void exec() = 0;
	void setHeaderContentType(std::string );
	void setHeaderContentLength(long);

	std::string getResponseMsg() const;
};
