#include "unittests.hpp"

std::vector<Parsing::server> *createVirtualHosts()
{
	std::vector<Parsing::server> *servers = new std::vector<Parsing::server>();
	Parsing::server server;
	server.host = "127.0.0.1";
	server.port = 8080;
	server.names = std::vector<std::string>{"localhost"};
	server.root = "/tmp/";
	server.locations.push_back(Parsing::location());
	server.locations[0].name = std::string("/");
	server.locations[0].root = "/tmp";
    server.locations[0].methods = std::vector<std::string>{"GET", "HEAD", "POST", "PUT", "TRACE", "DELETE"};
	servers->push_back(server);
	return servers;
}

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		std::string arg = std::string(argv[1]);
		if (arg.compare("-r") == 0)
			testRequest();
		else if (arg.compare("-c") == 0)
			testCGI();
		else if (arg.compare("-re") == 0)
			testResponse();
		else if (arg.compare("-p") == 0)
			testParsing();
	}else{
		testRequest();
		//testCGI();
		testParsing();
	}
//	testResponseBuild();
	return 0;
}
