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

//void addListener(const std::string &name, const std::string &ip, int port)
void addListener(const Parsing::servers &server)
{
	Listener *l = new Listener(inet_addr(server.host.c_str()),
						   server.port,
						   server.names[0]);
	Log().Get(logINFO) << server.names[0] << " started on port " << server.host << ":" << server.port << " (maxconn: " << FD_SETSIZE << ")";
	l->ListenAndServe();
	Server::getInstance()->addFileDescriptor(l);
}

int main(int argc, char *argv[]) {
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT, signalHandler);
	if (argc > 1 && std::string(argv[1]).compare("-v") == 0)
		Log::setLevel(logDEBUG);
	Server *webserv = Server::getInstance();
	Parsing p = Parsing("./parsing/test/nginx.conf");
	p.parseConfig();
	for (size_t i = 0; i < p.getServers().size(); i++)
		addListener(p.getServers()[i]);
	webserv->start();
}

