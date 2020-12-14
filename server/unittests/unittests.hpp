#pragma once

#include "tests.hpp"
#include "../fds/Client.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
//#include "CGI.hpp"

#define BADREQUEST 1
#define BADMETHOD 2
#define BADVERSION 3
#define BADHEADERNAME 4
#define BADHEADER 5
#define BADBODY 6

void requestLine()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	
	std::string str = "GET /qwe HTTP/1.1\r\n";
	Request r(str);
	assertEqual(r.parse(), 0, "correct line");

	r.reset();
	r.request_ = "GETGET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), BADMETHOD, "method written twice");
	
	r.reset();
	r.request_ = "GET GET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), BADREQUEST, "4 spaces");

	r.reset();
	r.request_ = "GET /qweHTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), BADREQUEST, "only one space");

	r.reset();
	r.request_ = "GET /qwe HTTP1.1\r\n\r\n";
	assertEqual(r.parse(), BADVERSION, "version badly written");
}

void sequencialReceive()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	size_t start = 0;
	size_t len = 5;
	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	std::string get;

	Request r(get);
	while ((start + len) < str.size())
	{
	  	get.append(str, start, len);
		start += len;
	}
	get.append(str, start);
	assertEqual(r.parse(), 0, "correct request sent, sequence reading");
}

void receivedAtOnce()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	Request r(str);

	assertEqual(r.parse(), 0, "correct request");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "wrong content-length header");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "no content-length header");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\nGET /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADHEADER, "2 correct request line");

	r.reset();
	r.request_ = "get /qwe HTTP/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADMETHOD, "lowercase method");

	r.reset();
	r.request_ = "GET /qwe http/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), BADVERSION, "lowercase version");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\n\r\nmessage";
	assertEqual(r.parse(), BADBODY, "1 body no headers");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\nhost: url\r\nuser-agent: hop\r\ndate: today\r\ncontent-length: 7\r\nmessage";
	assertEqual(r.parse(), BADHEADERNAME, "no newline bet headers and body");

	r.reset();
	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\ndate: today\r\ncontent-length: 7\r\nmessage";
	r.parse();
	assertEqual(r.queryString_parsed, true, "parsing query_string");
}

void testParsingRequest()
{
	sequencialReceive();
	receivedAtOnce();
	requestLine();
}

//void testClient()
//{
//	Client c(0, 0);
//
//	std::string get = "GET / HTTP/1.1\r\n\r\n";
//	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
//	assertStringEqual(c.getResponse(), get, "Client GET");
//}

