#include "Server.hpp"
#include "Log.hpp"
#include "fds/Configuration.hpp"
#include <signal.h>

void deleteSingletons() {
	delete (Parsing::getInstance());
	delete (Server::getInstance());
	delete (Mime::getInstance());
	delete (Log::getInstance());
}

void signalHandler(int) {
	std::cerr.clear();
	std::cerr << "\b\b";
	Log::get(logINFO) << "Webserver exiting gracefully." << std::endl;
	exit(0);
}

int main(int argc, char *argv[]) {
	std::string				*confPath;

	if (argc > 2)
	{
		std::cerr << "To many arguments default usage : webserv [./example.conf]" << std::endl;
		return (EXIT_FAILURE);
	}
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
	if (argc == 1)
		confPath = new std::string(DEFAULT_PATH);
	else	
		confPath = new std::string(argv[1]);
	//Log::getInstance()->setLevel(logDEBUG);
	Configuration *conf = new Configuration(*confPath);
	delete(confPath);
	if (conf->openFile() != 0) {
		delete conf;
		deleteSingletons();
		return (EXIT_FAILURE);
	}
	Server *webserv = Server::getInstance();
	if (Mime::getInstance()->getFd() > 0)
		webserv->addFileDescriptor(Mime::getInstance());
	webserv->addFileDescriptor(conf);
	webserv->start();
}

