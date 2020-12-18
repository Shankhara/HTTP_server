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

void addListener(const Parsing::server &server, std::vector<Listener*> listeners)
{
	for (unsigned long i = 0; i < listeners.size(); i++)
	{
		if (listeners[i]->addServer(server) == 0)
			return ;
	}
	Listener *l = new Listener();
	l->addServer(server);
	listeners.push_back(l);
}

void startListeners(const std::vector<Listener*> listeners)
{
	for (unsigned long i = 0; i < listeners.size(); i++)
	{
		listeners[i]->ListenAndServe();
		Server::getInstance()->addFileDescriptor(listeners[i]);
	}
}

int main(int argc, char *argv[]) {
	std::vector<Listener*> listeners;

	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
	std::string conf("./parsing/test/wordpress.conf");
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0)
		Log::setLevel(logDEBUG);
	else if (argc > 1)
		conf = std::string(argv[1]);
	Parsing p = Parsing(conf);
	try {
		p.parseConfig();
	} catch (Parsing::ParsingException &e) {
		Log().Get(logERROR) << " Unable to parse: " << e.what();
		return (EXIT_FAILURE);
	}
	Server *webserv = Server::getInstance();
	for (size_t i = 0; i < p.getServers().size(); i++)
		addListener(p.getServers()[i], listeners);
	startListeners(listeners);
	webserv->start();
}

