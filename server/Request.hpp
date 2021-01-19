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
#define MAX_HEADER_SIZE 8192

class Request
{
	private:
	int statusCode_;
    int totalHeaderSize_;
    unsigned int headerContentLength_;
	bool traceDebug_;
    std::string request_;
	std::string tracePayload_;
	std::string msgBody_;
	std::string queryString_;
	std::string originalReqTarget_;
	std::string headerDate_;
	std::string headerAuth_;
	std::string headerHost_;
	std::string headerReferer_;
	std::string headerUserAgent_;
	std::string headerContentType_;
    std::string headerTransferEncoding_;
    std::vector<std::string> headersRaw_;
    std::vector<std::string> requestLine_;
    std::vector<std::string> cgiHeaders_;
    std::vector<std::string> headerAcceptLanguage_;
    std::vector<std::string> headerContentLanguage_;
    const std::vector<Parsing::server> &servers_;
    const Parsing::location *location_;
    const Parsing::server *server_;

    int accessControl_();
	bool isMethodAuthorized_(const Parsing::location *) const;
	bool isAuthenticated_(const Parsing::location *) const;
	const Parsing::location	*matchLocation_(const Parsing::server *) const;
	const Parsing::server *matchServer_() const;
	bool checkMethodCase_();
	bool checkReqTarget_();
	bool checkUpload_();

	public:
	Request(const std::vector<Parsing::server> &);
	~Request();
	bool headers_parsed;
    bool requestIndexed;
	std::vector<std::string> methods;
    std::vector<std::string> headersName;
    enum e_headerLine { HEADERTITLE, HEADERCONTENT };
	enum e_RequestLine { METHOD, REQTARGET, VERSION };
	enum e_headers { ACCEPT_CHARSET, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, CONTENT_LANGUAGE, \
		CONTENT_LENGTH, CONTENT_LOCATION, CONTENT_TYPE, DATE, HOST, LAST_MODIFIED, LOCATION, REFERER, \
		TRANSFER_ENCODING, USER_AGENT };
	int parse();
    int doRequest(char[], size_t);
	int parseRequestLine();
	int parseHeaders();
	int checkMethod();
	int checkVersion();
	int parseBody();
	int getChunkedBody();
	int parseHeadersContent();
	void parseQueryString();

	std::vector<std::string> workLine(std::string &, const char &);
	std::string decode_authorization();

	int getStatusCode() const;
	int getHeaderContentLength() const;
	std::string getTracePayload() const;
	std::string getRequest() const;
	std::string getMethod() const;
	std::string getReqTarget() const;
	std::string getQueryStr() const;
	std::string getHeaderDate() const;
	std::string getHeaderAuth() const;
	std::string getHeaderHost() const;
	std::string getHeaderReferer() const;
    std::string getHeaderContentType() const;
	std::vector<std::string> getHeaderAcceptLanguage() const;
	std::vector<std::string> getHeaderContentLanguage() const;
    const Parsing::server *getServer() const;
    const Parsing::location *getLocation() const;
    const std::string &getBody() const;
	const std::string &getHeaderUserAgent() const;
	const std::string &getOriginalReqTarget() const;
    const std::vector<std::string> &getCGIHeaders() const;

};
