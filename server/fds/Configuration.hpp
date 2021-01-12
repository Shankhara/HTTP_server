#ifndef WEBSERV_CONFIGURATION_HPP
#define WEBSERV_CONFIGURATION_HPP
#include "FileDescriptor.hpp"
#include "../parsing/Parsing.hpp"
#include "../Server.hpp"
#include "Listener.hpp"

class Configuration: public  FileDescriptor{
public:
	virtual ~Configuration();
	Configuration(std::string configPath);
	void onEvent();
	int openFile();
	void shutdown_();

private:
	std::string configPath_;
	void 		addListener(const Parsing::server &server, std::vector<Listener*> *listeners);
	void 		startListeners(const std::vector<Listener*> *listeners);
};


#endif //WEBSERV_CONFIGURATION_HPP
