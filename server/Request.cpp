#include "Request.hpp"
#include "Utils.hpp"

Request::Request(Client & c) : client(c)
{
	headersRaw_.resize(11);
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
		if ((i = line.find('\r')) == std::string::npos)
			return (res); // Pas de CR
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
		headersRaw_[dist] = headerLine[HEADERCONTENT];
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
		if (!headersRaw_[CONTENT_LENGTH].empty())
			return (1); // Ce header implique un body
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
	std::vector<std::string> res;

	res = explode(headersRaw_[AUTHORIZATION], ' ');
	if (res[0] == "Basic")
		res[1] = decodeBase64(res[1]);
	//else
		//Print ERROR : type unknown
	return (res[1]);
}

void Request::parseHeadersContent()
{
	//GENERAL HEADERS
	headerDate_ = headersRaw_[DATE];

	//REQUEST HEADERS
	headerAcceptCharset_ = explode(headersRaw_[ACCEPT_CHARSETS], ',');
	headerAcceptLanguage_ = explode(headersRaw_[ACCEPT_LANGUAGE], ',');
	headerAuth_ = decode_authorization();
	headerHost_ = headersRaw_[HOST];//DNS:PORT (Error 400 BadRequest if not present)
	headerReferer_ = headersRaw_[REFERER];//URL

	//ENTITY HEADERS
	headerAllow_ = explode(headersRaw_[ALLOW], ',');
	headerContentLanguage_ = explode(headersRaw_[CONTENT_LANGUAGE], ',');
	headerContentLength_ = headersRaw_[CONTENT_LENGTH];//decimal
	headerContentLocation_ = headersRaw_[CONTENT_LOCATION];//URL
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
