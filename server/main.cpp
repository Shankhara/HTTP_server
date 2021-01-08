#include "Server.hpp"
#include "Log.hpp"
#include "fds/Listener.hpp"
#include "parsing/Parsing.hpp"
#include <signal.h>

void signalHandler(int) {
	std::cerr.clear();
	std::cerr << "\b\b";
	Log::get(logINFO) << "Webserver exiting gracefully." << std::endl;
	delete (Parsing::getInstance());
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

void startListeners(const std::vector<Listener*> *listeners)
{
	for (uint64_t i = 0; i < listeners->size(); i++)
	{
		listeners->at(i)->ListenAndServe();
		Server::getInstance()->addFileDescriptor(listeners->at(i));
	}
}

int main(int argc, char *argv[]) {
<<<<<<< HEAD
	std::vector<Listener*>	listeners;
	Parsing					p;
	std::string				conf;
=======
	std::vector<Listener*> *listeners = new std::vector<Listener *>();
>>>>>>> f7729de404dca4f3a7d2676eb97e376eed097ed7

	if (argc > 2)
	{
		std::cerr << "To many argument default usage : webserv [example.conf]" << std::endl;
		return (EXIT_FAILURE);
	}
	if (argc == 2)
		conf = std::string(argv[1]);
	else	
		conf = std::string("./parsing/test/wordpress.conf");
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
<<<<<<< HEAD
=======
	std::string *confPath = new std::string("./parsing/test/wordpress.conf");
>>>>>>> f7729de404dca4f3a7d2676eb97e376eed097ed7
	//Log::getInstance()->setLevel(logDEBUG);
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0) {
		Log::getInstance()->setLevel(logDEBUG);
	} else if (argc > 1) {
		confPath->assign(argv[1]);
	}
<<<<<<< HEAD
	p = Parsing(conf);
=======
	Parsing::getInstance()->setFile(*confPath);
	delete(confPath);
>>>>>>> f7729de404dca4f3a7d2676eb97e376eed097ed7
	try {
		Parsing::getInstance()->parseConfig();
	} catch (Parsing::ParsingException &e) {
		Log::get(logERROR) << " Unable to parse: " << e.what() << std::endl;
		delete listeners;
		delete Parsing::getInstance();
		delete Log::getInstance();
		return (EXIT_FAILURE);
	}
	Server *webserv = Server::getInstance();
	for (size_t i = 0; i < Parsing::getInstance()->getServers().size(); i++) {
		addListener(Parsing::getInstance()->getServers()[i], listeners);
	}
	startListeners(listeners);
	delete (listeners);
	webserv->start();
}

