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

	setBaseHeaders();
}

Response::~Response() { }

//std::string Response::getContentType()
//{
//	
//}

void Response::setBaseHeaders()
{
	headersToPut_.push_back("Server: Webserv");
	headersToPut_.push_back("Date: " + getStrDate());
}

void Response::setHeaderContentType(std::string value) {
	headersToPut_.push_back("Content-Type: " + value);
}

void Response::setHeaderContentLength(long value) {
	headersToPut_.push_back("Content-Length: " + ft_itoa(value));
}

void Response::putHeaders()
{
	msg_.append("\r\n");
	for (size_t i = 0; i < headersToPut_.size(); i++)
	{
		msg_.append(headersToPut_[i]);
		msg_.append("\r\n");
	}
	msg_.append("\r\n");
}

void Response::createResponse()
{
	msg_.append(req_.getVersion());
	msg_.append(" ");
	msg_.append(ft_itoa(statusCode_));
	msg_.append(" ");
	msg_.append(statusMap_[statusCode_]);
	putHeaders();
}


std::string Response::getResponseMsg() const
{ return msg_; }
