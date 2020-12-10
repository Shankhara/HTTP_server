#include "Request.hpp"
#include "Utils.hpp"

Request::Request(std::string & request) : request_(request)
{
	headersRaw_.resize(11);
	methods = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE", "PATCH" };
	headersName = { "accept-charsets", "accept-language", "allow", \
	"authorization", "content-language", "content-length", "content-location", \
	"content-type", "date", "host", "referer", };
}

//Request::Request(Client & c) : client_(c), request_(c.getResponse())
//{
//	headersRaw_.resize(11);
//	methods = { "GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS", "TRACE", "PATCH" };
//	headersName = { "accept-charsets", "accept-language", "allow", \
//	"authorization", "content-language", "content-length", "content-location", \
//	"content-type", "date", "host", "referer", };
//}

Request::~Request()
{

}

//int Request::getQueryString()
//{
//    int i = 0;
//
//	i = requestLine_[REQTARGET].find('?');
//	if (i != std::string::npos)
//	{
//		queryString_ = requestLine_[REQTARGET].substr(i);
//		requestLine_[REQTARGET].erase(i);
//	}
//}

std::string Request::decodeBase64(std::string & str)
{
	int val = 0;
	int valb = -8;
	unsigned char c;
	std::string res;
	std::vector<int> tab(256, -1);
	std::string::iterator it = str.begin();

	for (int i = 0; i < 64; i++)
		tab["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 
	while (it != str.end())
	{
		c = *it;
		if (tab[c] == -1)
			break;
		val = (val << 6) + tab[c];
		valb += 6;
		if (valb >= 0)
		{
			res.push_back(char((val >> valb) & 0xFF));
			valb -= 8;
		}
		it++;
	}
	return res;
}

std::string Request::decode_authorization()
{
	std::vector<std::string> tmp;
	std::string res;

	tmp = explode(headersRaw_[AUTHORIZATION], ' ');
	if (tmp[0] == "Basic")
		res = decodeBase64(tmp[1]);
	//else
		//Print ERROR : type unknown
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

int Request::checkVersion()
{
	if (requestLine_[VERSION] != "HTTP/1.1")
		return (1);
	return (0);
}

int Request::checkHeadersEnd()
{
	if (request_ == "\r\n")
		return (1);
	return (0);
}

int Request::getBody()
{
	std::string line;
	
	if (!headersRaw_[CONTENT_LENGTH].empty())
	{
		msgBody_ = request_;
		size_t len = atoi(headersRaw_[CONTENT_LENGTH].c_str());
		if (msgBody_.size() == len)
			return (0);
	}
	return (1);	
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
				return (0);
			else
				return (2);
		}

  		std::string::iterator st = headerLine[HEADERTITLE].begin();
  		std::string::iterator ste = headerLine[HEADERTITLE].end();

  		std::transform(st, ste, st, [](unsigned char c)-> unsigned char { return std::tolower(c); });
		itx = std::find(it, ite, headerLine[HEADERTITLE]);
		if (itx == ite)
			return (1);
		dist = std::distance(it, itx);
		headersRaw_[dist] = headerLine[HEADERCONTENT];
	}
	return (1);
}

int Request::parseRequestLine()
{
	std::string line;

	getNextLine(request_, line);
	requestLine_ = workLine(line, ' ');

	if (requestLine_.size() != 3)
		return (1);
	if (checkMethod())
	    return (2);
	if (checkVersion())
		return (3);
	return (0);
}

int Request::parse()
{
	static int i = 0;
    if (!i && !parseRequestLine())
	{
		std::cout << requestLine_[METHOD] << std::endl;
		std::cout << requestLine_[REQTARGET] << std::endl;
		std::cout << requestLine_[VERSION] << std::endl;
		i++;
	}
	if ((i == 1) && !parseHeaders())
	{
		std::cout << headersRaw_[REFERER] << std::endl;
		std::cout << headersRaw_[CONTENT_TYPE] << std::endl;
		std::cout << headersRaw_[CONTENT_LENGTH] << std::endl;
		i++;
	}
	if ((i == 2) && !getBody())
	{
		std::cout << msgBody_ << std::endl;
		i++;
	}
	if (i > 1)
	{
		parseHeadersContent();
		i++;
	}
	return (i);
}

void Request::parseHeadersContent()
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
		headerAuth_ = decode_authorization();
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
}

std::vector<std::string> Request::getRequestLine()
{ return (requestLine_); }

std::string Request::getHeaderDate()
{ return (headerDate_); }

std::string Request::getHeaderAuth()
{ return (headerAuth_); }

std::string Request::getHeaderHost()
{ return (headerHost_); }

std::string Request::getHeaderReferer()
{ return (headerReferer_); }

std::string Request::getHeaderContentLength()
{ return (headerContentLength_); }

std::string Request::getHeaderContentLocation()
{ return (headerContentLocation_); }

std::vector<std::string> Request::getHeaderAcceptCharset()
{ return (headerAcceptCharset_); }

std::vector<std::string> Request::getHeaderAcceptLanguage()
{ return (headerAcceptLanguage_); }

std::vector<std::string> Request::getHeaderAllow()
{ return (headerAllow_); }

std::vector<std::string> Request::getHeaderContentLanguage()
{ return (headerContentLanguage_); }

std::vector<std::string> Request::getHeaderContentType()
{ return (headerContentType_); }

//Client &Request::getClient()
//{ return client_; }
