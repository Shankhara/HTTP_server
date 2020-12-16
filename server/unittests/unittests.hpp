#pragma once

#include "tests.hpp"
#include "../fds/Client.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
#include "../Response.hpp"
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
	
	Request r;
	r.request_ = "GET /qwe HTTP/1.1\r\n";
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

	Request r;
	while ((start + len) < str.size())
	{
	  	get.append(str, start, len);
		r.request_ = get;
		start += len;
	}
	get.append(str, start);
	r.request_ = get;
	assertEqual(r.parse(), 0, "correct request sent, sequence reading");
}

void testConstructRequest()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;
	Request r;
	std::string req("GET / HTTP/1.1\r\n\r\n");
	r.appendRequest(const_cast<char *>(req.c_str()), 16);
	r.parse();
	assertEqual(r.getStatusCode(), 100, "Request not over");
	r.appendRequest(const_cast<char *>(req.c_str()) + 16, 2);
	r.parse();
	assertEqual(r.getStatusCode(), 200, "Request over");
}

void receivedAtOnce()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

  	Request r;
	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
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
	assertEqual(r.parse(), BADHEADER, "no newline bet headers and body");

	r.reset();
	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\ndate: today\r\ncontent-length: 7\r\nmessage";
	r.parse();
	assertEqual(r.queryString_parsed, true, "parsing query_string");

	r.reset();
	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\ndate: today\r\n";
	assertEqual(r.parse(), BADHEADER, "no newline after headers with no body");

	r.reset();
	r.request_ = "GET /qwe?name=client&date=today HTTP/1.1\r\nauthorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l\r\n\r\n";
	r.parse();
	assertStringEqual(r.getHeaderAuth(), "aladdin:opensesame", "correct parsing authorization");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux \r\n12\r\nsans saut de ligne\r\n0\r\n\r\n";
	assertEqual(r.parse(), 0, "get correct chunked body");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n0\r\n\r\n";
	assertEqual(r.parse(), BADBODY, "get bad chunked body (bad chunk size)");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n0\n\r\n";
	assertEqual(r.parse(), BADBODY, "get bad chunked body (bad end)");

	r.reset();
	r.request_ = "GET /qwe HTTP/1.1\r\ntransfer-encoding: chunked\r\n\r\n26\r\nVoici les données du premier morceau\r\n1C\r\net voici un second morceau\r\n20\r\net voici deux derniers morceaux aaaaaaaaaaaaaaa12\r\nsans saut de ligne\r\n4\r\n\r\n";
	assertEqual(r.parse(), BADBODY, "get bad chunked body (bad end 2)");
}

void testRequestParse()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	testConstructRequest();
	sequencialReceive();
	receivedAtOnce();
	requestLine();
}

void statusLine()
{
	std::cout << std::endl << "\033[1;33m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	Request r;

	r.request_ = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	r.parse();

	Response a(r);
	a.createResponse();

	std::cout << a.getResponseMsg() << std::endl;
}

void testResponseBuild()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__ << "\033[0m" << std::endl;

	statusLine();
}
