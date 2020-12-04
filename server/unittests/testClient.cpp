#include "../Client.hpp"
#include "../Request.hpp"
#include "tests.hpp"

void testRequest()
{
	Client c(0);
	Request r(c);

	std::string get = "GET /http://localhost:8080 HTTP/1.1\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
	assertEqual(r.parse(), 1, "parseRequestLine");
}

void testClient()
{
	Client c(0);

	std::string get = "HTTP 1.1 GET /\r\n\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
	assertStringEqual(c.getResponse(), get, "Client GET");
}
