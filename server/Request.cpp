#include "Request.hpp"
#include "Utils.hpp"

Request::Request()
{
	headersRaw_.resize(18);
	requestLine_parsed = false;
	headers_parsed = false;
	body_parsed = false;
	queryString_parsed = false;
	statusCode_ = 100;

	static const std::string str_list[8] = {"GET", "HEAD", "POST", "PUT", "DELETE", \
		"OPTIONS", "TRACE", "PATCH" };
	std::vector<std::string> tmp(str_list, str_list + 8);
	methods = tmp;

	static const std::string str_list2[18] = { "accept-charsets", "accept-language", "allow", \
		"authorization", "content-language", "content-length", "content-location", \
		"content-type", "date", "host", "last-modified", "location", "referer", \
		"retry-after", "server", "transfer-encoding", "user-agent", "www-authenticate" };
	std::vector<std::string> tmp2(str_list2, str_list2 + 18);
	headersName = tmp2;
}

Request::~Request() { }

void Request::reset()
{
	requestLine_parsed = false;
	headers_parsed = false;
	body_parsed = false;
	queryString_parsed = false;
	statusCode_ = 100;

	request_.clear();
	requestLine_.clear();
	headersRaw_.clear();
	msgBody_.clear();
	queryString_.clear();

	headerDate_.clear();
	headerAuth_.clear();
	headerHost_.clear();
	headerReferer_.clear();
	headerContentLength_.clear();
	headerContentLocation_.clear();

	headerAcceptCharset_.clear();
	headerAcceptLanguage_.clear();
	headerAllow_.clear();
	headerContentLanguage_.clear();
	headerContentType_.clear();
}

std::string Request::decode_authorization()
{
	std::vector<std::string> tmp;
	std::string res;

	tmp = explode(headersRaw_[AUTHORIZATION], ' ');
	if (tmp[0] == "Basic")
		res = decode64(tmp[1]);
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
		hex_size.assign(request_, start, end);
		num_size = strHex_to_int(hex_size);
		if (num_size > 8000)
			break;

		start = end + 2;
		if (request_[start + num_size] == '\n')
			num_size++;

		total_size += num_size;
		res.append(request_, start, num_size);
		start = request_.find_first_not_of("\r\n", start + num_size);
	}

	check.append(request_, start);
	if (check != "0\r\n\r\n")
		return (1);

	msgBody_ = res;
	return (0);
}

int Request::getBody()
{
	if (request_.size() > 0)
	{
		size_t pos = headersRaw_[TRANSFER_ENCODING].find("chunked");
		if (pos != std::string::npos)
		{
			if (getChunkedBody())
			{
				statusCode_ = 400;
				return (BADBODY);
			}
			body_parsed = 0;
			return (0);
		}
		if (headersRaw_[CONTENT_LENGTH].empty())
		{
			statusCode_ = 411;
			return (BADBODY);
		}

		msgBody_ = request_;
		size_t len = atoi(headersRaw_[CONTENT_LENGTH].c_str());
		
		if (msgBody_.size() == len)
		{
			body_parsed = 0;
			return (0);
		}
	}
	return (BADBODY);	
}

void Request::parseQueryString()
{
    size_t i = 0;

	i = requestLine_[REQTARGET].find('?');
	if (i != std::string::npos)
	{
		queryString_ = requestLine_[REQTARGET].substr(i);
		requestLine_[REQTARGET].erase(i, std::string::npos);
		queryString_parsed = 1;
	}
}

int Request::parseHeaders()
{
	int dist, ret;
	std::string line;
	std::vector<std::string> headerLine;
	std::vector<std::string>::iterator it = headersName.begin();
	std::vector<std::string>::iterator ite = headersName.end();
	std::vector<std::string>::iterator itx;
	
	while ((ret = getNextLine(request_, line)) > -1)
	{
		headerLine = workLine(line, ':');
		if (headerLine.empty())
		{
			if (line == "\r")
			{
				headers_parsed = 1;
				return (0);
			}
			break;
		}

  		std::string::iterator st = headerLine[HEADERTITLE].begin();
  		std::string::iterator ste = headerLine[HEADERTITLE].end();

  		std::transform(st, ste, st, ft_tolower);
		itx = std::find(it, ite, headerLine[HEADERTITLE]);
		if (itx != ite)
		{
			dist = std::distance(it, itx);
			headersRaw_[dist] = headerLine[HEADERCONTENT];
			if (headerLine[HEADERCONTENT].size() > 8000)//get larger_client_header_buffers)
			{
				statusCode_ = 414;
				break;
			}
		}
	}
	return (BADHEADER);
}

int Request::parseHeadersContent()
{
	//GENERAL HEADERS
	if (!headersRaw_[DATE].empty())
		headerDate_ = headersRaw_[DATE];

	//REQUEST HEADERS
	if (!headersRaw_[ACCEPT_CHARSETS].empty())
		headerAcceptCharset_ = explode(headersRaw_[ACCEPT_CHARSETS], ',');
	if (!headersRaw_[ACCEPT_LANGUAGE].empty())
		headerAcceptLanguage_ = explode(headersRaw_[ACCEPT_LANGUAGE], ',');
	if (!headersRaw_[AUTHORIZATION].empty())
	{
		headerAuth_ = decode_authorization();
		if (headerAuth_.empty())
		{
			statusCode_ = 401;
			return (1);
		}
	}
	if (!headersRaw_[HOST].empty())
		headerHost_ = headersRaw_[HOST];
	if (!headersRaw_[REFERER].empty())
		headerReferer_ = headersRaw_[REFERER];

	//ENTITY HEADERS
	if (!headersRaw_[ALLOW].empty())
		headerAllow_ = explode(headersRaw_[ALLOW], ',');
	if (!headersRaw_[CONTENT_LANGUAGE].empty())
		headerContentLanguage_ = explode(headersRaw_[CONTENT_LANGUAGE], ',');
	if (!headersRaw_[CONTENT_LENGTH].empty())
		headerContentLength_ = atoi(headersRaw_[CONTENT_LENGTH].c_str());
	if (!headersRaw_[CONTENT_LOCATION].empty())
		headerContentLocation_ = headersRaw_[CONTENT_LOCATION];
	if (!headersRaw_[CONTENT_TYPE].empty())
		headerContentType_ = explode(headersRaw_[CONTENT_TYPE], ';');
	return 0;
}

int Request::parseRequestLine()
{
	std::string line;

	getNextLine(request_, line);
	requestLine_ = workLine(line, ' ');

	if (requestLine_.size() != 3)
	{
		statusCode_ = 400;
		return (BADREQUEST);
	}
	if (checkMethod())
	{
		statusCode_ = 501;
	    return (BADMETHOD);
	}
	if (requestLine_.size() > 8000)//get larger_client_header_buffers)
	{
		statusCode_ = 414;
		return (BADREQUEST);
	}
	if (checkVersion())
	{
		statusCode_ = 505;
		return (BADVERSION);
	}
	requestLine_parsed = 1;
	return (0);
}

int Request::parse()
{
	int ret = 0;

    if (request_.size() && !requestLine_parsed)
	{
		if ((ret = parseRequestLine()))
			return (ret);
		if (!queryString_parsed)
			parseQueryString();
	}
	
	if (request_.size() && !headers_parsed)
		if ((ret = parseHeaders()))
			return (ret);

	if (request_.size() && !body_parsed)
		if ((ret = getBody()))
			return (ret);

	if (headers_parsed)
		ret = parseHeadersContent();

	return (ret);
}

int Request::appendRequest(char buf[256], int nbytes)
{
	request_.append(buf, nbytes);
	return (parse());
}

std::string Request::getMethod() const
{ return (requestLine_[METHOD]); }

std::string Request::getReqTarget() const
{ return (requestLine_[REQTARGET]); }

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

std::string Request::getHeaderContentLength() const
{ return (headerContentLength_); }

std::string Request::getHeaderContentLocation() const
{ return (headerContentLocation_); }

std::vector<std::string> Request::getHeaderAcceptCharset() const
{ return (headerAcceptCharset_); }

std::vector<std::string> Request::getHeaderAcceptLanguage() const
{ return (headerAcceptLanguage_); }

std::vector<std::string> Request::getHeaderAllow() const
{ return (headerAllow_); }

std::vector<std::string> Request::getHeaderContentLanguage() const
{ return (headerContentLanguage_); }

std::vector<std::string> Request::getHeaderContentType() const
{ return (headerContentType_); }
