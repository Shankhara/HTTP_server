#include "Request.hpp"

Request::Request() { }

Request::Request(Client c) : client(c) { }

Request::~Request() { }

int Request::checkMethod()
{
	std::vector<std::string> methods { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTION", "TRACE", "PATCH" };
	enum e_methods { GET, HEAD, POST, PUT, DELETE, OPTION, TRACE, PATCH };
	enum e_RequestLine { METHOD, URL, VERSION };
	int ret;
	size_t i = 0;	

	while (i < methods.size())
	{
		ret = requestLine_[METHOD].find(methods[i]);
		i++;
	}
	return (ret);
}

int Request::parseRequestLine()
{
	std::string line;

	getNextLine(request_, line);
	line.erase(line.find('\r'));
	requestLine_ = explode(line, ' ');
	
	if (requestLine_.size() != 3)
		return (0);
	
	if (checkMethod())
	    return (1);

	return (0);
}

int Request::parse()
{
	int ret = 0;
	
	request_ = client.getResponse();
	
    if ((ret = parseRequestLine()))
		return (ret);
	return (ret);
}
