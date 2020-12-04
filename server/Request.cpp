#include "Request.hpp"
#include "Utils.hpp"

Request::Request() { }

Request::Request(Client c) : client(c) { }

Request::~Request() { }

int Request::checkMethod()
{
	size_t ret, i = 0;	

	while (i < methods.size())
	{
		if ((ret = requestLine_[METHOD].find(methods[i])))
		{
			ret += methods[i].size();
			if (requestLine_[METHOD].at(ret))
				return (1);
			break;
		}
		i++;
	}
	return (0);
}

int Request::parseRequestLine()
{
	std::string line;

	getNextLine(request_, line);
	line.erase(line.find('\r'));
	requestLine_ = explode(line, ' ');
	
	if (requestLine_.size() != 3)
	{
		// LOG ERROR REQUESTLINE
		return (1);
	}
	if (checkMethod())
	{
		// LOG ERROR METHOD
	    return (1);
	}
	return (0);
}

void Request::parse()
{
	int ret = 0;
	
	request_ = client.getResponse();
	
    if (parseRequestLine())
		return;
	if (parseHeaders())
		return;
	return (ret);
}
