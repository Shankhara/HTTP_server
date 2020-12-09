#pragma once

#include <vector>
#include <algorithm>
#include "select/Client.hpp"

#define LEN 100
#define CRLF "\r\n"

class Client;

class Request
{
	private:
	std::string request_;
	std::vector<std::string> requestLine_;
	std::vector<std::string> headersRaw_;
	std::string msgBody_;
	
	std::string headerDate_;
	std::string headerAuth_;
	std::string headerHost_;
	std::string headerReferer_;
	std::string headerContentLength_;
	std::string headerContentLocation_;
	
	std::vector<std::string> headerAcceptCharset_;
	std::vector<std::string> headerAcceptLanguage_;
	std::vector<std::string> headerAllow_;
	std::vector<std::string> headerContentLanguage_;
	std::vector<std::string> headerContentType_;

	public:
	Request();
	Request(Client);
	~Request();
	
	Client client;
	std::vector<std::string> methods { "GET", "HEAD", "POST", "PUT", \
		"DELETE", "OPTIONS", "TRACE", "PATCH" };
	enum e_methods { GET, HEAD, POST, PUT, DELETE, OPTION, TRACE, PATCH };
	enum e_RequestLine { METHOD, REQTARGET, VERSION };

	std::vector<std::string> headersName { "accept-charsets", "accept-language", "allow", \
		"authorization", "content-language", "content-length", "content-location", \
		"content-type", "date", "host", "referer", };
	enum e_headers { ACCEPT_CHARSETS, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, CONTENT_LANGUAGE, \
		CONTENT_LENGTH, CONTENT_LOCATION, CONTENT_TYPE, DATE, HOST, REFERER };
	enum e_headerLine { HEADERTITLE, HEADERCONTENT };

	int parse();
	int parseRequestLine();
	int parseHeaders();
	int checkMethod();
	int checkHeadersEnd();
	int getBody();
	void parseHeadersContent();
	std::vector<std::string> workNextLine(std::string &, const char &);
	std::string decodeBase64(std::string &);
	std::string decode_authorization();
	
	std::vector<std::string> getRequestLine();
	std::string getHeaderDate();
	std::string getHeaderAuth();
	std::string getHeaderHost();
	std::string getHeaderReferer();
	std::string getHeaderContentLength();
	std::string getHeaderContentLocation();
	std::vector<std::string> getHeaderAcceptCharset();
	std::vector<std::string> getHeaderAcceptLanguage();
	std::vector<std::string> getHeaderAllow();
	std::vector<std::string> getHeaderContentLanguage();
	std::vector<std::string> getHeaderContentType();
};
