#include "Server.hpp"
#include "Logger.hpp"
#include <signal.h>



void signalHandler(int) {
	std::cerr << "\b\b";
	Log().Get(logINFO) << "Exiting gracefully.";
	delete (Server::getInstance());
	exit(0);
}

int main() {
	signal(SIGINT, signalHandler);
	//Log::SetLevel(logDEBUG);
	Server *webserv = Server::getInstance();
	webserv->addListener("webserver 0", "notUsed", 8080);
	webserv->addListener("webserver 1", "notUsed", 8081);
	webserv->addListener("webserver 2", "notUsed", 8082);
	webserv->start();
}

