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
	webserv->setName("webserv");
	webserv->setPort(8080);
	webserv->start();
}

