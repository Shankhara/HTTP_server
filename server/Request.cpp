#include "Request.hpp"

Request::Request(const std::vector<Parsing::server> &servers): servers_(servers)
{
	headersRaw_.resize(15);
	cgiHeaders_.reserve(30);
	headers_parsed = false;
	requestIndexed = false;
	statusCode_ = 100;
	location_ = 0;
	if (servers.size() == 1)
		server_ = &servers_[0];
	else
		server_ = 0;
	traceDebug_ = false;
	headerContentLength_ = 0;
	totalHeaderSize_ = 0;

	static const std::string str_list[7] = { "GET", "HEAD", "POST", "PUT", "DELETE", \
		"OPTIONS", "TRACE" };
	std::vector<std::string> tmp(str_list, str_list + 7);
	methods = tmp;

	static const std::string str_list2[15] = { "accept-charset", "accept-language", "allow", \
		"authorization", "content-language", "content-length", "content-location", \
		"content-type", "date", "host", "last-modified", "location", "referer", \
		"transfer-encoding", "user-agent" };
	std::vector<std::string> tmp2(str_list2, str_list2 + 15);
	headersName = tmp2;
}

Request::~Request() { }

std::string Request::decode_authorization()
{
    std::string res;
	std::vector<std::string> tmp = explode(headersRaw_[AUTHORIZATION], ' ');
	removeSpaces(tmp[0]);
	std::transform(tmp[0].begin(), tmp[0].end(), tmp[0].begin(), ft_tolower);
	if (tmp[0] == "basic")
		res = decode64(removeSpaces(tmp[1]));
	return (res);
}

std::vector<std::string> Request::workLine(std::string & line, const char & c)
{
	std::vector<std::string> res;
	size_t i = line.find(c);
	if (i == std::string::npos)
		return (res);
	
	if (!line.empty())
	{
		i = line.find('\r');
		if (i == std::string::npos)
			return (res);
		line.erase(line.begin() + i);
		if (c == ':')
			res = explode(line, c, 1);
		else
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
	std::string strHexChunkSize;
	size_t cursor, hexEndPos, chunkSize;

	if (request_.size() == 1 && request_[0] == '0' && msgBody_.empty())
		return 200;
	if (msgBody_.empty())
		msgBody_.reserve(100000000);
	while (request_.size() > 3 && request_[0] != '0')
	{
		hexEndPos = request_.find("\r\n", 0);
		if (hexEndPos == std::string::npos)
			return 100;
		strHexChunkSize.assign(request_, 0, hexEndPos);
		chunkSize = strHexToInt(strHexChunkSize);
		if (msgBody_.size() + chunkSize > location_->client_max_body_size)
			return (413);
		if (chunkSize > CHUNK_MAX_SIZE)
		{
			Log::get(logERROR) << __FUNCTION__ << " chunk_size too big " << chunkSize \
			<< " > " << CHUNK_MAX_SIZE << std::endl;
			return 400;
		}
		if (request_.size() < chunkSize + hexEndPos + 4)
			return 100;
		else
		{
			cursor = chunkSize + hexEndPos + 2;
			if (request_[cursor] != '\r' && request_[cursor + 1] != '\n')
				return (400);
			cursor += 2;
			msgBody_.append(request_, hexEndPos + 2, chunkSize);
			request_.assign(request_.c_str() + cursor);
		}
	}
	if (request_.size() < 5)
		return 100;
	if (request_ != "0\r\n\r\n")
	{
		Log::get(logERROR) << __FUNCTION__  << " not ending with expected sequence" << std::endl;
		return 400;
	}
	return 200;
}

int Request::parseBody()
{
	if (traceDebug_)
		return 400;
	if (headerTransferEncoding_ == "chunked")
		return getChunkedBody();
	else {
		size_t len = headerContentLength_;
		if (request_.size() > location_->client_max_body_size)
			return 413;
		else if (request_.size() == len)
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
    size_t i = requestLine_[REQTARGET].find('?');
	if (i != std::string::npos)
	{
		queryString_ = requestLine_[REQTARGET].substr(i + 1);
		requestLine_[REQTARGET].erase(i);
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
        if (totalHeaderSize_ + line.size() > MAX_HEADER_SIZE)
            return 400;
        totalHeaderSize_ += line.size();
		ret = line.find(':', 0);
		if (ret != std::string::npos && (line[ret - 1] < 33 || line[ret + 1] == ':'))
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
			if (headerLine[HEADERCONTENT].size() > CHUNK_MAX_SIZE)
				return 414;
			if (!headersRaw_[dist].empty())
			    return 400;
			headersRaw_[dist] = headerLine[HEADERCONTENT];
		}
		size_t notSpace = headerLine[HEADERCONTENT].find_first_not_of(' ', 0);
		if (notSpace != std::string::npos)
			cgiHeaders_.push_back(headerLine[HEADERTITLE] + ":" + headerLine[HEADERCONTENT].substr(notSpace, headerLine[HEADERCONTENT].size() - notSpace));
		else
			cgiHeaders_.push_back(headerLine[HEADERTITLE] + ":" + headerLine[HEADERCONTENT]);
	}
	return 400;
}

int Request::parseHeadersContent()
{
	//REQUEST HEADERS
	if (!headersRaw_[ACCEPT_CHARSET].empty())
	{
		std::string tmp = removeSpaces(headersRaw_[ACCEPT_CHARSET]);
  		std::transform(tmp.begin(), tmp.end(), tmp.begin(), ft_tolower);
  		std::vector<std::string> vec_tmp = explode(tmp, ',');
        for(size_t i = 0; i < vec_tmp.size(); ++i)
        {
            if (!vec_tmp[i].compare("utf-8") || !vec_tmp[i].compare("*"))
                break;
            if (i == vec_tmp.size() - 1)
                return 406;
        }
	}
	if (!headersRaw_[ACCEPT_LANGUAGE].empty())
    {
		headerAcceptLanguage_ = explode(removeSpaces(headersRaw_[ACCEPT_LANGUAGE]), ',');
		for(size_t i = 0; i < headerAcceptLanguage_.size(); i++)
		{
		    std::transform(headerAcceptLanguage_[i].begin(), headerAcceptLanguage_[i].end(),
		    headerAcceptLanguage_[i].begin(), ft_toupper);
		    size_t ret = headerAcceptLanguage_[i].find(';');
		    if (ret != std::string::npos)
		        headerAcceptLanguage_[i].erase(ret);
		}
    }
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
		headerUserAgent_ = removeSpaces(headersRaw_[USER_AGENT]);
	//ENTITY HEADERS
	if (!headersRaw_[CONTENT_LANGUAGE].empty())
    {
        headerContentLanguage_ = explode(removeSpaces(headersRaw_[CONTENT_LANGUAGE]), ',');
        for(size_t i = 0; i < headerContentLanguage_.size(); ++i)
        {
            std::transform(headerContentLanguage_[i].begin(), headerContentLanguage_[i].end(),
            headerContentLanguage_[i].begin(), ft_toupper);
            if (!isAlpha(headerContentLanguage_[i][0]))
                return 400;
            if (!isAlpha(headerContentLanguage_[i][headerContentLanguage_[i].size() - 1]))
                return 400;
        }
    }
	if (!headersRaw_[CONTENT_LENGTH].empty())
    {
        for(size_t i = 0; i < headersRaw_[CONTENT_LENGTH].size(); ++i)
        {
            if (headersRaw_[CONTENT_LENGTH][i] != ' ')
                if (!isDigit(headersRaw_[CONTENT_LENGTH][i]))
                    return 400;
        }
        headerContentLength_ = ft_atoi(removeSpaces(headersRaw_[CONTENT_LENGTH]));
    }
	if (!headersRaw_[CONTENT_TYPE].empty())
		headerContentType_ = removeSpaces(headersRaw_[CONTENT_TYPE]);
    //RESPONSE HEADER
	if (!headersRaw_[TRANSFER_ENCODING].empty())
	{
		if (!headersRaw_[CONTENT_LENGTH].empty())
			return 400;
		headerTransferEncoding_ = removeSpaces(headersRaw_[TRANSFER_ENCODING]);
	}
	headers_parsed = true;
	if ((statusCode_ = accessControl_()) >= 300)
		return statusCode_;
	if (headersRaw_[CONTENT_LENGTH].empty() && headersRaw_[TRANSFER_ENCODING].empty())
		return 200;
	return 100;
}

int Request::accessControl_()
{
	originalReqTarget_ = requestLine_[REQTARGET];

	if (server_ == 0)
		server_ = matchServer_();
	location_ = matchLocation_(server_);
	if (location_ == 0)
		return 403;
	if (checkUpload_())
		return 405;
	if (!isMethodAuthorized_(location_))
		return 405;
	if (!isAuthenticated_(location_))
		return 401;
	if (location_->name == requestLine_[REQTARGET] + "/")
		return 301;

	if (!location_->root.empty())
	{
		requestLine_[REQTARGET] = std::string(requestLine_[REQTARGET], \
		location_->name.size(), requestLine_[REQTARGET].size() - 1);
		if (requestLine_[REQTARGET][0] != '/')
			requestLine_[REQTARGET] = '/' + requestLine_[REQTARGET];
	}
	if (!location_->index.empty() && !location_->autoindex && requestLine_[REQTARGET][requestLine_[REQTARGET].size() -1] == '/')
    {
		requestLine_[REQTARGET] += location_->index;
		requestIndexed = true;
    }
	return statusCode_;
}

int Request::parseRequestLine()
{
	std::string line;
	getNextLine(request_, line);
	requestLine_ = workLine(line, ' ');

	if (requestLine_.size() != 3)
		return 400;
	if (checkMethodCase_()||checkReqTarget_())
		return 400;
	if (checkMethod())
		return 501;
	if (requestLine_.size() > CHUNK_MAX_SIZE)
		return 414;
	if (checkVersion())
		return 505;
	if (getMethod() == "TRACE")
	{
		traceDebug_ = true;
		tracePayload_.assign(line + "\r\n" + request_);
	}
	parseQueryString();
	if (request_ == "\r\n")
		return (200);
	return (100);
}

int Request::parse()
{
	if (!headers_parsed && boolFind(request_, "\r\n\r\n"))
	{
		statusCode_ = parseRequestLine();
		if (statusCode_ == 200)
			statusCode_ = accessControl_();
		if (statusCode_ == 100)
			statusCode_ = parseHeaders();
	}
	if (headers_parsed && statusCode_ == 100)
		statusCode_ = parseBody();
	return (statusCode_);
}

int Request::doRequest(char buf[], size_t nbytes)
{
	request_.append(buf, nbytes);
	parse();
	return (statusCode_);
}

const Parsing::server *Request::matchServer_() const
{
	std::string host;
	size_t pos = headerHost_.find(':', 0);
	if (pos != std::string::npos)
		host.assign(headerHost_, 0, pos);
	else
		host = headerHost_;
	for (unsigned long i = 0; i < servers_.size(); i++)
	{
		for (unsigned long k = 0; k < servers_[i].names.size(); k++)
		{
			if (servers_[i].names[k] == host)
				return (&servers_[i]);
		}
	}
	return (&servers_[0]);
}

const Parsing::location *Request::matchLocation_(const Parsing::server *server) const
{
	const Parsing::location *location = 0;
	unsigned long minSize = 0;

	for (unsigned long j = 0; j < server->locations.size(); j++)
	{
		if (std::string(getReqTarget() + "/") == server->locations[j].name)
			return (&server->locations[j]);
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

bool Request::isAuthenticated_(const Parsing::location *location) const
{
	if (location->auth_basic.empty() || location->auth_basic == "off")
		return true;
	if (headerAuth_ == SERVER_CREDS)
		return true;
	return false;
}

bool 	Request::isMethodAuthorized_(const Parsing::location *location) const
{
	if (location->methods.size() == 0)
		return true;
	for (unsigned long i = 0; i < location->methods.size(); i++)
	{
		if (location->methods[i] == getMethod())
		{
			return true;
		}
	}
	return false;
}

const Parsing::server *Request::getServer() const
{ return server_; }

const Parsing::location *Request::getLocation() const
{ return location_; }

int Request::getStatusCode() const
{ return (statusCode_); }

const std::string &Request::getBody() const
{ return (msgBody_); }

std::string Request::getTracePayload() const
{ return (tracePayload_); }

std::string Request::getMethod() const
{
	if (!requestLine_.empty())
		return (requestLine_[METHOD]);
	return "";
}

std::string Request::getReqTarget() const
{
	if (requestLine_.size() > 1)
		return (requestLine_[REQTARGET]);
	return "";
}

std::string Request::getQueryStr() const
{
	if (requestLine_.size() > 2)
		return (queryString_);
	return "";
}

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

std::vector<std::string> Request::getHeaderAcceptLanguage() const
{ return (headerAcceptLanguage_); }

std::vector<std::string> Request::getHeaderContentLanguage() const
{ return (headerContentLanguage_); }

std::string Request::getHeaderContentType() const
{ return (headerContentType_); }

const std::vector<std::string> &Request::getCGIHeaders() const
{ return cgiHeaders_; }

const std::string &Request::getOriginalReqTarget() const
{ return originalReqTarget_; }

const std::string &Request::getHeaderUserAgent() const
{ return headerUserAgent_; }

bool Request::checkMethodCase_() {
	for (size_t i = 0; i < requestLine_[METHOD].size(); i++){
		if (requestLine_[METHOD][i] < 'A' || requestLine_[METHOD][i] > 'Z')
			return true;
	}
	return false;
}

bool Request::checkReqTarget_() {
	size_t pos = requestLine_[REQTARGET].find("..");
	if (pos == std::string::npos)
		return false;
	return true;
}

bool Request::checkUpload_() {
	if ((getMethod() == "POST"|| getMethod() == "PUT") && !location_->upload_enable)
		return false;
	return true;
}
