#include "Server.hpp"
#include "Logger.hpp"
#include "fds/Listener.hpp"
#include "parsing/Parsing.hpp"
#include <signal.h>

void signalHandler(int) {
	std::cerr << "\b\b";
	Log().Get(logINFO) << "Webserver exiting gracefully.";
	delete (Server::getInstance());
	exit(0);
}

void addListener(const Parsing::servers &server)
{
	Listener *l = new Listener();
	l->addServer(server);
	l->ListenAndServe();
	Server::getInstance()->addFileDescriptor(l);
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0)
		Log::setLevel(logDEBUG);
	Parsing p = Parsing("./parsing/test/nginx.conf");
	try {
		p.parseConfig();
	} catch (Parsing::ParsingException &e) {
		Log().Get(logERROR) << " Unable to parse: " << e.what();
		return (EXIT_FAILURE);
	}
	Server *webserv = Server::getInstance();
	for (size_t i = 0; i < p.getServers().size(); i++)
		addListener(p.getServers()[i]);
	webserv->start();
}

