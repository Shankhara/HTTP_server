#pragma once

#include "tests.hpp"
#include "../fds/Client.hpp"
#include "../Server.hpp"
#include "../Request.hpp"
//#include "CGI.hpp"

void testRequest()
{
//	size_t start = 0;
//	size_t len = 5;
	std::string str = "GET /qwe HTTP/1.1\r\nContent-length: 12\r\nReferer: 2\r\nContent-Type: 3\r\n\r\nmessage-body";
	std::string get;

	Request r(get);
//	while ((start + len) < str.size())
//	{
		get.append(str);//, start, len);
//		std::cout << "h: " << get << std::endl;
		assertEqual(r.parse(), 4, "Parsing Request");
//		start += len;
//	}
}

//void testClient()
//{
//	Client c(0, 0);
//
//	std::string get = "GET / HTTP/1.1\r\n\r\n";
//	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
//	assertStringEqual(c.getResponse(), get, "Client GET");
//}

