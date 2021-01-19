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
	//server.locations[0].auth_basic = std::string("Admin");
	server.locations[0].root = "/tmp";
	server.locations[0].client_max_body_size = 500000;
    server.locations[0].methods = std::vector<std::string>{"GET", "HEAD", "POST", "PUT", "TRACE", "DELETE", "OPTIONS"};
	servers->push_back(server);
	return servers;
}

int main(int argc, char *argv[])
{
	Log::getInstance()->setLevel(logERROR);
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
		testParsing();
		testRequest();
		//testCGI();
		//testResponse();
	}
	return 0;
}
