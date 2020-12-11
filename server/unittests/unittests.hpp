#pragma once

#include "tests.hpp"
#include "../fds/Client.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
//#include "CGI.hpp"

void testParsingRequestLine()
{
	std::cout << std::endl << __FUNCTION__ << std::endl;
	std::string str = "GET /qwe HTTP/1.1\r\n";
	Request r(str);
	assertEqual(r.parse(), 0, "correct line");

	str = "GETGET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), 1, "method written twice");
	
	str = "GET GET /qwe HTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), 1, "4 spaces");

	str = "GET /qweHTTP/1.1\r\n\r\n";
	assertEqual(r.parse(), 1, "only one space");

	str = "GET /qwe HTTP1.1\r\n\r\n";
	assertEqual(r.parse(), 1, "version badly written");
}

void testParsingRequestSequence()
{
	std::cout << std::endl << __FUNCTION__ << std::endl;
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

void testParsingRequestReceivedAtOnce()
{
	std::cout << std::endl << __FUNCTION__ << std::endl;
	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	Request r(str);

	assertEqual(r.parse(), 0, "correct request");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), 1, "wrong content-length header");

	r.reset();
	r.request_ = "get /qwe http/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), 1, "non content-length header");

	r.reset();
	r.request_ = "get /qwe http/1.1\r\nget /qwe http/1.1\r\nreferer: 2\r\ncontent-type: 3\r\n\r\nmessage";
	assertEqual(r.parse(), 1, "2 request line");
}

//void testClient()
//{
//	Client c(0, 0);
//
//	std::string get = "GET / HTTP/1.1\r\n\r\n";
//	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
//	assertStringEqual(c.getResponse(), get, "Client GET");
//}

