#pragma once

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>

#define CRLF "\r\n"

#define BADREQUEST 1
#define BADMETHOD 2
#define BADVERSION 3
#define BADHEADERNAME 4
#define BADHEADER 5
#define BADBODY 6

class Request
{
	friend class Response;

	private:
	std::vector<std::string> requestLine_;
	std::vector<std::string> headersRaw_;
	int statusCode_;
	std::string msgBody_;
	std::string queryString_;
	
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
	~Request();
	
	std::string request_;
	bool requestLine_parsed;
	bool headers_parsed;
	bool body_parsed;
	bool queryString_parsed;

	std::vector<std::string> methods;
	enum e_methods { GET, HEAD, POST, PUT, DELETE, OPTION, TRACE, PATCH };
	enum e_RequestLine { METHOD, REQTARGET, VERSION };

	std::vector<std::string> headersName;
	enum e_headers { ACCEPT_CHARSETS, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, CONTENT_LANGUAGE, \
		CONTENT_LENGTH, CONTENT_LOCATION, CONTENT_TYPE, DATE, HOST, LAST_MODIFIED, LOCATION, REFERER, \
		RETRY_AFTER, SERVER, TRANSFER_ENCODING, USER_AGENT, WWW_AUTHENTICATE };
	enum e_headerLine { HEADERTITLE, HEADERCONTENT };

	int appendRequest(char [256], int);
	int parse();
	int parseRequestLine();
	int parseHeaders();
	int checkMethod();
	int checkVersion();
	int checkHeadersEnd();
	int getBody();
	int getChunkedBody();
	int parseHeadersContent();
	void parseQueryString();
	void reset();
	void replaceReturnCarriage(std::string & str);

	std::vector<std::string> workLine(std::string &, const char &);
	std::string decodeBase64(std::string &);
	std::string decode_authorization();

	int getStatusCode() const;
	std::string getMethod() const;
	std::string getReqTarget() const;
	std::string getVersion() const;
	std::string getHeaderDate() const;
	std::string getHeaderAuth() const;
	std::string getHeaderHost() const;
	std::string getHeaderReferer() const;
	std::string getHeaderContentLength() const;
	std::string getHeaderContentLocation() const;
	std::vector<std::string> getHeaderAcceptCharset() const;
	std::vector<std::string> getHeaderAcceptLanguage() const;
	std::vector<std::string> getHeaderAllow() const;
	std::vector<std::string> getHeaderContentLanguage() const;
	std::vector<std::string> getHeaderContentType() const;
};
