#pragma once

#include <vector>
#include "Client.hpp"
#include <algorithm>
#include <locale>

#define LEN 100
#define CRLF "\r\n\r\n"

class Client;

class Request
{
	private:
	std::string request_;
	std::vector<std::string> requestLine_;
	std::vector<std::string> headers_;

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
		"content-type", "date", "host", "last-modified", "location", "referer", \
		"retry-after", "server", "transfer-encoding", "user-agent", "www-authenticate" };
	enum e_headers { ACCEPT_CHARSETS, ACCEPT_LANGUAGE, ALLOW, AUTHORIZATION, \
		CONTENT_LANGUAGE, CONTENT_LENGTH, CONTENT_LOCATION, CONTENT_TYPE, DATE, HOST, \
		LAST_MODIFIED, LOCATION, REFERER, RETRY_AFTER, SERVER, TRANSFER_ENCODING, USER_AGENT, \
		WWW_AUTHENTICATE };
	enum e_headerLine { HEADERTITLE, HEADERCONTENT };

	int parse();
	int parseRequestLine();
	int parseHeaders();
	int checkMethod();
	std::vector<std::string> workNextLine(std::string &, const char &);
};
