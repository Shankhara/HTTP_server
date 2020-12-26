#include "Request.hpp"

Parsing::server * Request::server_ = NULL;

Request::Request(std::vector<Parsing::server> &servers): servers_(servers)
{
	headersRaw_.resize(18);
	headers_parsed = false;
	statusCode_ = 100;
	headerContentLength_ = 0;

	static const std::string str_list[9] = { "CONNECT", "GET", "HEAD", "POST", "PUT", "DELETE", \
		"OPTIONS", "TRACE", "PATCH" };
	std::vector<std::string> tmp(str_list, str_list + 9);
	methods = tmp;

	static const std::string str_list2[15] = { "accept-charsets", "accept-language", "allow", \
		"authorization", "content-language", "content-length", "content-location", \
		"content-type", "date", "host", "last-modified", "location", "referer", \
		"transfer-encoding", "user-agent" };
	std::vector<std::string> tmp2(str_list2, str_list2 + 15);
	headersName = tmp2;
}

Request::~Request() { }

std::string Request::decode_authorization()
{
	std::vector<std::string> tmp;
	std::string res;

	tmp = explode(headersRaw_[AUTHORIZATION], ' ');
	removeSpaces(tmp[0]);
	if (tmp[0] == "Basic")
		res = decode64(removeSpaces(tmp[1]));
	return (res);
}

std::vector<std::string> Request::workLine(std::string & line, const char & c)
{
	std::vector<std::string> res;
	size_t i;

	i = line.find(c);
	if (i == std::string::npos)
		return (res);
	
	if (!line.empty())
	{
		i = line.find('\r');
		if (i == std::string::npos)
			return (res);

		line.erase(line.begin() + i);
		res = explode(line, c);
	}
	return (res);
}

int Request::checkMethod()
{
	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == requestLine_[METHOD])
				return (0);
	}
	return (1);
}

int Request::checkVersion()
{
	if (requestLine_[VERSION] != "HTTP/1.1")
		return (1);
	return (0);
}

int Request::getChunkedBody()
{
	std::string hex_size, res, check;
	size_t start = 0, end = 0, num_size = 0, total_size = 0;

	while (start == 0 || request_[start] != '0')
	{
		end = request_.find("\r\n", start);
		hex_size.assign(request_, start, end - start);
		num_size = strHex_to_int(hex_size);
		if (num_size > MAX_SIZE)
			return 400;

		start = end + 2;
		if (request_[start + num_size] == '\n')
			num_size++;

		total_size += num_size;
		res.append(request_, start, num_size);
		start = request_.find_first_not_of("\r\n", start + num_size);
	}

	check.append(request_, start);
	if (check != "0\r\n\r\n")
		return 400;

	msgBody_ = res;
	return 200;
}

int Request::parseBody()
{
	size_t ret = headerTransferEncoding_.find("chunked");
	if (ret != std::string::npos)
		return getChunkedBody();
	else
	{
		size_t len = headerContentLength_;
		if (request_.size() == len)
		{
			msgBody_ = request_;
			return 200;
		}
		if (request_.size() > len)
			return 400;
	}
	return 100;	
}

void Request::parseQueryString()
{
    size_t i = 0;

	i = requestLine_[REQTARGET].find('?');
	if (i != std::string::npos)
	{
		queryString_ = requestLine_[REQTARGET].substr(i);
		requestLine_[REQTARGET].erase(i, std::string::npos);
	}
}

int Request::parseHeaders()
{
	size_t dist, ret;
	std::string line;
	std::vector<std::string> headerLine;
	std::vector<std::string>::iterator itx, it = headersName.begin();
	std::vector<std::string>::iterator ite = headersName.end();
	
	while ((getNextLine(request_, line)) > -1)
	{
		ret = line.find(':', 0);
		if (ret != std::string::npos && line[ret - 1] < 33)
			return 400;

		headerLine = workLine(line, ':');
		if (headerLine.empty())
		{
			if (line == "\r")
				return parseHeadersContent();
			return 400;
		}

  		std::string::iterator st = headerLine[HEADERTITLE].begin();
  		std::string::iterator ste = headerLine[HEADERTITLE].end();

  		std::transform(st, ste, st, ft_tolower);
		itx = std::find(it, ite, headerLine[HEADERTITLE]);
		if (itx != ite)
		{
			dist = std::distance(it, itx);
			if (headerLine[HEADERCONTENT].size() > MAX_SIZE)
				return 414;
			headersRaw_[dist] = headerLine[HEADERCONTENT];
		}
	}
	return 400;
}

int Request::parseHeadersContent()
{
	//GENERAL HEADERS
	if (!headersRaw_[DATE].empty())
		headerDate_ = removeSpaces(headersRaw_[DATE]);

	//REQUEST HEADERS
	if (!headersRaw_[ACCEPT_CHARSETS].empty())
		headerAcceptCharset_ = removeSpaces(headersRaw_[ACCEPT_CHARSETS]);
	if (!headersRaw_[ACCEPT_LANGUAGE].empty())
		headerAcceptLanguage_ = removeSpaces(headersRaw_[ACCEPT_LANGUAGE]);
	if (!headersRaw_[AUTHORIZATION].empty())
	{
		headerAuth_ = decode_authorization();
		if (headerAuth_.empty())
			return 401;
	}
	if (!headersRaw_[HOST].empty())
		headerHost_ = removeSpaces(headersRaw_[HOST]);
	else
		return 400;
	if (!headersRaw_[REFERER].empty())
		headerReferer_ = removeSpaces(headersRaw_[REFERER]);
	if (!headersRaw_[USER_AGENT].empty())
		headerTransferEncoding_ = removeSpaces(headersRaw_[USER_AGENT]);

	//ENTITY HEADERS
	if (!headersRaw_[ALLOW].empty())
		headerAllow_ = removeSpaces(headersRaw_[ALLOW]);
	if (!headersRaw_[CONTENT_LANGUAGE].empty())
		headerContentLanguage_ = removeSpaces(headersRaw_[CONTENT_LANGUAGE]);
	if (!headersRaw_[CONTENT_LENGTH].empty())
		headerContentLength_ = atoi(removeSpaces(headersRaw_[CONTENT_LENGTH]).c_str());
	if (!headersRaw_[CONTENT_LOCATION].empty())
		headerContentLocation_ = removeSpaces(headersRaw_[CONTENT_LOCATION]);
	if (!headersRaw_[CONTENT_TYPE].empty())
		headerContentType_ = removeSpaces(headersRaw_[CONTENT_TYPE]);

	//RESPONSE HEADERS
	if (!headersRaw_[LAST_MODIFIED].empty())
		headerTransferEncoding_ = removeSpaces(headersRaw_[LAST_MODIFIED]);
	if (!headersRaw_[LOCATION].empty())
		headerTransferEncoding_ = removeSpaces(headersRaw_[LOCATION]);

	if (!headersRaw_[TRANSFER_ENCODING].empty())
		headerTransferEncoding_ = removeSpaces(headersRaw_[TRANSFER_ENCODING]);

	headers_parsed = true;

	if (headersRaw_[CONTENT_LENGTH].empty() && headersRaw_[TRANSFER_ENCODING].empty())
		return 200;

	return (100);
}

int Request::parseRequestLine()
{
	std::string line;

	getNextLine(request_, line);
	requestLine_ = workLine(line, ' ');

	if (requestLine_.size() != 3)
		return 400;
	if (checkMethod())
		return 400; //pas 501 car on implemente toutes les methodes
	if (requestLine_.size() > MAX_SIZE)
		return 414;
	if (checkVersion())
		return 505;

	parseQueryString();

	if (request_ == "\r\n")
		return (200);

	return (100);
}

int Request::parse()
{
	if (boolFind(request_, "\r\n\r\n"))
	{
		statusCode_ = parseRequestLine();
		if (statusCode_ == 100)
			statusCode_ = parseHeaders();
	}
	if (headers_parsed && statusCode_ == 100)
		statusCode_ = parseBody();

	if (statusCode_ == 200)
	{
		server_ = matchServer_();
		location_ = matchLocation_(server_);
		if (location_ == 0)
			statusCode_ = 403;
		else if (!isMethodAuthorized_(location_))
			statusCode_ = 405;
	}
	return (statusCode_);
}

int Request::doRequest(char buf[], size_t nbytes)
{
	request_.append(buf, nbytes);

	parse();

	return (statusCode_);
}

Parsing::server *Request::matchServer_() const
{
	for (unsigned long i = 0; i < servers_.size(); i++)
	{
		for (unsigned long k = 0; k < servers_[i].names.size(); k++)
		{
			if (servers_[i].names[k].compare(getHeaderHost()) == 0)
				return (&servers_[i]);
		}
	}
	return (&servers_[0]);
}

Parsing::location *Request::matchLocation_(Parsing::server *server) const
{
	Parsing::location *location = 0;
	unsigned long minSize = 0;

	for (unsigned long j = 0; j < server->locations.size(); j++)
	{
		if (getReqTarget().rfind(server->locations[j].name, 0) == 0)
		{
			if (server->locations[j].name.size() > minSize)
			{
				location = &server->locations[j];
				minSize = server->locations[j].name.size();
			}
		}
	}
	return (location);
}

bool 	Request::isMethodAuthorized_(Parsing::location *location) const
{
	if (location->methods.size() == 0)
		return true;
	for (unsigned long i = 0; i < location->methods.size(); i++)
	{
		if (location->methods[i].compare(getMethod()) == 0)
		{
			return true;
		}
	}
	return false;
}

Parsing::server *Request::getServer() const
{ return server_; }

Parsing::location *Request::getLocation() const
{ return location_; }

int Request::getStatusCode() const
{ return (statusCode_); }

std::string Request::getBody() const
{ return (msgBody_); }

std::string Request::getRequest() const
{ return (requestLine_[METHOD]); }

std::string Request::getMethod() const
{ return (requestLine_[METHOD]); }

std::string Request::getReqTarget() const
{ return (requestLine_[REQTARGET]); }

std::string Request::getQueryStr() const
{ return (queryString_); }

std::string Request::getVersion() const
{ return (requestLine_[VERSION]); }

std::string Request::getHeaderDate() const
{ return (headerDate_); }

std::string Request::getHeaderAuth() const
{ return (headerAuth_); }

std::string Request::getHeaderHost() const
{ return (headerHost_); }

std::string Request::getHeaderReferer() const
{ return (headerReferer_); }

int Request::getHeaderContentLength() const
{ return (headerContentLength_); }

std::string Request::getHeaderContentLocation() const
{ return (headerContentLocation_); }

std::string Request::getHeaderAcceptCharset() const
{ return (headerAcceptCharset_); }

std::string Request::getHeaderAcceptLanguage() const
{ return (headerAcceptLanguage_); }

std::string Request::getHeaderAllow() const
{ return (headerAllow_); }

std::string Request::getHeaderContentLanguage() const
{ return (headerContentLanguage_); }

std::string Request::getHeaderContentType() const
{ return (headerContentType_); }
