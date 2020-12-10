#pragma once

#include "tests.hpp"
#include "../fds/Client.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
//#include "CGI.hpp"

void testRequest()
{
	Client c(0, 0);

	std::string get = "GET /qwe HTTP/1.1\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	Request r(c);
	int len = 0;
	int getLen = get.size();
//	while (len < getLen)
//	{
		len += 5;
		c.constructRequest(const_cast<char *>(get.c_str()), len);
		assertEqual(r.parse(), 0, "Parsing Request");
		get.erase(0, len);
		std::cout << get << std::endl;
//	}
}

void testClient()
{
//	Client c;

	std::string get = "GET / HTTP/1.1\r\n\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
	assertStringEqual(c.getResponse(), get, "Client GET");
}
