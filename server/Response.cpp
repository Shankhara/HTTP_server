#include "Response.hpp"

Response::Response(const Request & r) : request_(r)
{

}

Response::~Response() { }

void Response::statusLine()
{
	msg_.append(request_.requestLine_[Request::VERSION]);
}

void Response::createResponse()
{
	statusLine();
}

std::string Response::getResponseMsg() const
{ return msg_; }
