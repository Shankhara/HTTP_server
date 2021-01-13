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
	Server::getInstance()->stop();
}

int main(int argc, char *argv[]) {
	Configuration *conf;

	if (argc > 2)
	{
		std::cerr << "Usage : webserv [./example.conf]" << std::endl;
		return (EXIT_FAILURE);
	}
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
	if (argc == 1)
		conf = new Configuration(DEFAULT_PATH);
	else	
		conf = new Configuration(argv[1]);
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
	deleteSingletons();
}

