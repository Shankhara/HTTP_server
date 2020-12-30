#pragma once

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdio.h>
#include "parsing/Parsing.hpp"
#include "Utils.hpp"
#include "Logger.hpp"

#define CHUNK_MAX_SIZE 65536

class Request
{
	private:
	int statusCode_;
	std::string request_;
	std::string msgBody_;
	std::string queryString_;
	std::vector<std::string> headersRaw_;
	std::vector<std::string> requestLine_;
	std::vector<Parsing::server> &servers_;
	Parsing::location *location_;
	Parsing::server *server_;

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

	std::string headerAcceptCharset_;
	std::string headerAcceptLanguage_;
	std::string headerAllow_;
	std::string headerContentLanguage_;
	std::string headerContentType_;

	bool				isMethodAuthorized_(Parsing::location *) const;
	Parsing::location	*matchLocation_(Parsing::server *) const;
	Parsing::server		*matchServer_() const;


	public:
	Request(std::vector<Parsing::server> &);
	~Request();
	
	bool requestLine_parsed;
	bool headers_parsed;

	bool isHeadersParsed() const;

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

	int doRequest(char[], size_t);
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
	void replaceReturnCarriage(std::string & str);

	std::vector<std::string> workLine(std::string &, const char &);
	std::string decode_authorization();

	Parsing::server *getServer() const;
	Parsing::location *getLocation() const;
	int getStatusCode() const;
	int getHeaderContentLength() const;
	std::string getBody() const;
	std::string consumeBody();
	std::string getRequest() const;
	std::string getMethod() const;
	std::string getReqTarget() const;
	std::string getQueryStr() const;
	std::string getVersion() const;
	std::string getHeaderDate() const;
	std::string getHeaderAuth() const;
	std::string getHeaderHost() const;
	std::string getHeaderReferer() const;
	std::string getHeaderContentLocation() const;
	std::string getHeaderAcceptCharset() const;
	std::string getHeaderAcceptLanguage() const;
	std::string getHeaderAllow() const;
	std::string getHeaderContentLanguage() const;
	std::string getHeaderContentType() const;
};
