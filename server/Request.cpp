#include "Request.hpp"
#include "Utils.hpp"

Request::Request()
{
	headers_.resize(18);
}

Request::Request(Client c) : client(c)
{
	headers_.resize(18);
}

Request::~Request()
{

}

std::vector<std::string> Request::workNextLine(std::string & line, const char & c)
{
	std::vector<std::string> res;
	size_t i;

	getNextLine(request_, line);
	if (!line.empty())
	{
		if ((i = line.find('\r') == std::string::npos))
			return (res); // Pas de CRLF en fin de ligne
		line.erase(i);
		res = explode(line, c);
	}
	return (res);
}

int Request::checkMethod()
{
	size_t ret, i = 0;	

	while (i < methods.size())
	{
		if ((ret = requestLine_[METHOD].find(methods[i])))
			if (requestLine_[METHOD].size() == methods[i].size())
				return (0);
		i++;
	}
	return (1);
}

int Request::parseRequestLine()
{
	std::string line;
	
	requestLine_ = workNextLine(line, ' ');
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

int Request::parseHeaders()
{
	int dist;
	std::string line;
	std::vector<std::string> headerLine;
	std::vector<std::string>::iterator it = headersName.begin();
	std::vector<std::string>::iterator ite = headersName.end();
	std::vector<std::string>::iterator itx;

	while (!(headerLine = workNextLine(line, ':')).empty())
	{
  		std::string::iterator st = headerLine[HEADERTITLE].begin();
  		std::string::iterator ste = headerLine[HEADERTITLE].end();

  		std::transform(st, ste, st, [](unsigned char c)-> unsigned char { return std::tolower(c); });
		itx = std::find(it, ite, headerLine[HEADERTITLE]);
		if (itx == ite)
			return (1); // En cas de flag non conforme
		dist = std::distance(it, itx);
		headers_[dist] = headerLine[HEADERCONTENT];
		//LOG FLAG PARSE SUCCESS
	}
	return (0);
}

int Request::checkHeadersEnd()
{
	if (request_.find("\r\n\r\n") == std::string::npos)
		return (1);
	return (0);
}

int Request::getBody()
{
	std::string line;

	if (request_.empty())
	{
		if (!headers_[CONTENT_LENGTH].empty() || !headers_[TRANSFER_ENCODING].empty())
			return (1); // Ces deux headers impliquent un body
	}
	else
	{
		getNextLine(request_, line);
		msgBody_ = line;
	}
	return (0);	
}

int Request::parse()
{
	request_ = client.getResponse();
	
	if (checkHeadersEnd())
		return (1);
    if (parseRequestLine())
		return (1);
	if (parseHeaders())
		return (1);
	if (getBody())
		return (1);
	return (0);
}
