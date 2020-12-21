#include "unittests.hpp"

std::vector<Parsing::server> *createVirtualHosts()
{
	std::vector<Parsing::server> *servers = new std::vector<Parsing::server>();
	Parsing::server server;
	server.host = "127.0.0.1";
	server.port = 8080;
	server.names = std::vector<std::string>{"localhost"};
	server.root = "/opt/server";
	server.locations.push_back(Parsing::location());
	server.locations[0].name = std::string("/");
	servers->push_back(server);
	return servers;
}

int main()
{
	testRequest();
	testCGI();
	testParsing();
//	testResponseBuild();
	return 0;
}
