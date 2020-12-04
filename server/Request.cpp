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
				return (0);
			break;
		}
		i++;
	}
	return (1);
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
	
	if (checkURL())
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
