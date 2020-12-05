#include "Server.hpp"
#include "Logger.hpp"
#include <signal.h>

void signalHandler(int) {
	std::cerr << "\b\b";
	Log().Get(logINFO) << "Webserver exiting gracefully.";
	delete (Server::getInstance());
	exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, signalHandler);
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0)
		Log::setLevel(logDEBUG);
	Server *webserv = Server::getInstance();
	webserv->addListener("Webserver 0", "notUsed", 8080);
	webserv->addListener("Webserver 1", "notUsed", 8081);
	webserv->addListener("Webserver 2", "notUsed", 8082);
	webserv->start();
}

