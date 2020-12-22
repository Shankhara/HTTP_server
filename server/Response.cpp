#include "Response.hpp"

Response::Response(const Request & r) : req_(r)
{
	statusCode_ = req_.getStatusCode();

	statusMap_[200] = "OK";
	statusMap_[201] = "Created";
	statusMap_[202] = "Accepted";
	statusMap_[204] = "No Content";
	statusMap_[400] = "Bad Request";
	statusMap_[401] = "Unauthorized";
	statusMap_[404] = "Not Found";
	statusMap_[405] = "Method Not Allowed";
	statusMap_[413] = "Request Entity Too Large";
	statusMap_[414] = "Request-URI Too Long";
	statusMap_[500] = "Internal Server Error";
}

Response::~Response() { }

void setHeaders()
{
	headersToPut_.push_back("Server: Webserv");
	headersToPut_.push_back("Date: " + getStrDate());
	headersToPut_.push_back("Content-Type: " + getContentType());
	headersToPut_.push_back("Content-Length: " + getContentLen());

}

void putHeaders()
{
	setHeaders();
	for (size_t i = 0; i < headersToPut_.size(); i++)
	{
		msg.append(headersToPut_[i]);
		msg.append("\r\n");
	}

}

void Response::createResponse()
{
	msg_.append(req_.getVersion());
	msg_.append(statusCode_);
	msg_.append(statusMap_[statusCode_]);
	putHeaders();
}

std::string Response::getResponseMsg() const
{ return msg_; }
