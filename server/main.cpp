#include "Server.hpp"
#include "Log.hpp"
#include "fds/Listener.hpp"
#include "parsing/Parsing.hpp"
#include <signal.h>

void signalHandler(int) {
	std::cerr.clear();
	std::cerr << "\b\b";
	Log::get(logINFO) << "Webserver exiting gracefully." << std::endl;
	delete (Server::getInstance());
	delete (Mime::getInstance());
	delete (Log::getInstance());
	exit(0);
}

void addListener(const Parsing::server &server, std::vector<Listener*> *listeners)
{
	for (uint64_t i = 0; i < listeners->size(); i++)
	{
		if (listeners->at(i)->addServer(server) == 0) {
			return;
		}
	}
	Listener *l = new Listener();
	l->addServer(server);
	listeners->push_back(l);
}

void startListeners(const std::vector<Listener*> &listeners)
{
	for (uint64_t i = 0; i < listeners.size(); i++)
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
	Log::getInstance()->setLevel(logDEBUG);
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0) {
		Log::getInstance()->setLevel(logDEBUG);
	} else if (argc > 1) {
		conf = std::string(argv[1]);
	}
	Parsing p = Parsing(conf);
	try {
		p.parseConfig();
	} catch (Parsing::ParsingException &e) {
		Log::get(logERROR) << " Unable to parse: " << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	Server *webserv = Server::getInstance();
	for (size_t i = 0; i < p.getServers().size(); i++) {
		addListener(p.getServers()[i], &listeners);
	}
	startListeners(listeners);
	webserv->start();
}

