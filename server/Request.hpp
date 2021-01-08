#pragma once

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdio.h>
#include "parsing/Parsing.hpp"
#include "Utils.hpp"
#include "Log.hpp"

#define CHUNK_MAX_SIZE 65536
#define SERVER_CREDS "user42:user42"

class Request
{
	private:
	int statusCode_;
	std::string request_;
	bool traceDebug_;
	std::string tracePayload_;
	std::string msgBody_;
	std::string queryString_;
	std::vector<std::string> 			headersRaw_;
	std::vector<std::string> 			requestLine_;
	std::vector<std::string>			cgiHeaders_;
    std::vector<std::string> headerAcceptLanguage_;
    std::map<std::string, std::string> languageLocation_;
	const std::vector<Parsing::server>	&servers_;
	const Parsing::location *location_;
	const Parsing::server *server_;
	std::string originalReqTarget_;
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
	std::string headerAllow_;
	std::vector<std::string> headerContentLanguage_;
	std::string headerContentType_;

	bool							isMethodAuthorized_(const Parsing::location *) const;
	bool 							isAuthenticated_(const Parsing::location *) const;
	const Parsing::location			*matchLocation_(const Parsing::server *) const;
	const Parsing::server			*matchServer_() const;
	int								accessControl_();


	public:
	Request(const std::vector<Parsing::server> &);
	~Request();
	
	bool requestLine_parsed;
	bool headers_parsed;

	bool isHeadersParsed() const;

	bool body_parsed;
	bool queryString_parsed;
	bool bodyToParse;


	std::vector<std::string> methods;
	enum e_methods { GET, HEAD, POST, PUT, DELETE, OPTION, TRACE };
	enum e_RequestLine { METHOD, REQTARGET, VERSION };

	std::vector<std::string> headersName;

	const std::vector<std::string> &getHeadersName() const;

	enum e_headers { ACCEPT_CHARSET, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, CONTENT_LANGUAGE, \
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

	const Parsing::server *getServer() const;
	const Parsing::location *getLocation() const;
	int getStatusCode() const;
	int getHeaderContentLength() const;
	const std::string &getBody() const;
	std::string getTracePayload() const;
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
    std::string getHeaderAllow() const;
	std::vector<std::string> getHeaderAcceptLanguage() const;
	std::vector<std::string> getHeaderContentLanguage() const;
	std::string getHeaderContentType() const;
	const std::string &getHeaderUserAgent() const;
	const std::vector<std::string> &getCGIHeaders() const;
	const std::string &getOriginalReqTarget() const;
};
