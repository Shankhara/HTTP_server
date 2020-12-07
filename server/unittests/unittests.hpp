#pragma once

#include "tests.hpp"
#include "../Client.hpp"
#include "../Request.hpp"

void testRequest()
{
	Client c(0);

	std::string get = "GET /http://localhost:8080 HTTP/1.1\r\nUser-Agent: 1\r\nReferer: 2\r\nContent-Type: 3\r\n\r\n";
//FAIL	std::string get = "GET /http://localhost:8080 HTTP/1.1\r\nUser-Agent: 1\r\nReferer: 2\r\nContent-Type: 3\r\nBonjour\r\n";
//FAIL	std::string get = "GETGET /http://localhost:8080 HTTP/1.1\r\n";
//FAIL	std::string get = "GET GET /http://localhost:8080 HTTP/1.1\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());

	Request r(c);
	assertEqual(r.parse(), 0, "parseRequestLine");
}

void testClient()
{
	Client c(0);

	std::string get = "HTTP 1.1 GET /\r\n\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
	assertStringEqual(c.getResponse(), get, "Client GET");
}
