#pragma once

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "parsing/Parsing.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

#define MAX_SIZE 8192

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
	std::vector<Parsing::server>  		&servers_;
	std::vector<std::string> requestLine_;
	std::vector<std::string> headersRaw_;
	int statusCode_;
	std::string msgBody_;
	std::string queryString_;
	Parsing::location *location_;
	
	int headerContentLength_;
	std::string headerDate_;
	std::string headerAuth_;
	std::string headerHost_;
	std::string headerReferer_;
	std::string headerContentLocation_;
	std::string headerTransferEncoding_;
	std::string headerUserAgent_;
	std::string headerLastModified_;
	std::string headerLocation_;

	std::vector<std::string> headerAcceptCharset_;
	std::vector<std::string> headerAcceptLanguage_;
	std::vector<std::string> headerAllow_;
	std::vector<std::string> headerContentLanguage_;
	std::vector<std::string> headerContentType_;

	bool isAuthorized_();
	void setLocation_();
	Parsing::server &matchServer_();


	public:
	Request(std::vector<Parsing::server> &);
	~Request();
	
	std::string request_;
	bool requestLine_parsed;
	bool headers_parsed;
	bool body_parsed;
	bool queryString_parsed;
	bool bodyToParse;

	std::vector<std::string> methods;
	enum e_methods { CONNECT, GET, HEAD, POST, PUT, DELETE, OPTION, TRACE, PATCH };
	enum e_RequestLine { METHOD, REQTARGET, VERSION };

	std::vector<std::string> headersName;
	enum e_headers { ACCEPT_CHARSETS, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, CONTENT_LANGUAGE, \
		CONTENT_LENGTH, CONTENT_LOCATION, CONTENT_TYPE, DATE, HOST, LAST_MODIFIED, LOCATION, REFERER, \
		TRANSFER_ENCODING, USER_AGENT };
	enum e_headerLine { HEADERTITLE, HEADERCONTENT };

	int appendRequest(char [256], int);
	int doRequest(char[256], size_t);
	int parse();
	int parseRequestLine();
	int parseHeaders();
	int checkMethod();
	int checkVersion();
	int checkHeadersEnd();
	int parseBody();
	int getChunkedBody();
	int parseHeadersContent();
	void parseQueryString();
//	void clear();
	void replaceReturnCarriage(std::string & str);

	std::vector<std::string> workLine(std::string &, const char &);
	std::string decode_authorization();

	int getStatusCode() const;
	int getHeaderContentLength() const;
	std::string getMethod() const;
	std::string getReqTarget() const;
	std::string getVersion() const;
	std::string getHeaderDate() const;
	std::string getHeaderAuth() const;
	std::string getHeaderHost() const;
	std::string getHeaderReferer() const;
	std::string getHeaderContentLocation() const;
	std::vector<std::string> getHeaderAcceptCharset() const;
	std::vector<std::string> getHeaderAcceptLanguage() const;
	std::vector<std::string> getHeaderAllow() const;
	std::vector<std::string> getHeaderContentLanguage() const;
	std::vector<std::string> getHeaderContentType() const;
};
