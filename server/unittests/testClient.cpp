#include "../Client.hpp"
#include "tests.hpp"

void testClient() {
	Client c(0);

	std::string get = "HTTP 1.1 GET /\r\n\r\n";
	c.constructRequest(const_cast<char *>(get.c_str()), get.size());
	assertStringEqual(c.getResponse(), get, "Client GET");
}
