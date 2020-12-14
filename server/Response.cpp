#include "Response.hpp"

Response::Response(const Request & r) : request_(r)
{

}

Response::~Response() { }

void Response::statusLine()
{
	msg_.append(request_.requestLine_[Request::VERSION]);
	msg_.append(" ");
}

void Response::statusCode()
{
//	msg_append(request_.statusCode);
	msg_.append(" ");
}

void Response::createResponse()
{
	statusLine();
	statusCode();
}

std::string Response::getResponseMsg() const
{ return msg_; }
