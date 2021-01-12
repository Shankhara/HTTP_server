#include "Configuration.hpp"

Configuration::Configuration(std::string path): configPath_(path) {}

Configuration::~Configuration() {}

void Configuration::onEvent() {
	char buf[8192];
	int		ret;
	std::string config;
	while ((ret = read(this->getFd(), buf, 8191)) > 0)
		config.append(buf, ret);
	if (ret == -1)
		shutdown_();
	Parsing::getInstance()->setFile(configPath_);
	Parsing::getInstance()->setContent(config);
	try {
		Parsing::getInstance()->parseConfig();
	} catch (const Parsing::ParsingException &e) {
		Log::get(logERROR) << e.what() << std::endl;
		shutdown_();
		exit(EXIT_FAILURE);
	}
	std::vector<Listener*> listeners;
	for (size_t i = 0; i < Parsing::getInstance()->getServers().size(); i++) {
		addListener(Parsing::getInstance()->getServers()[i], &listeners);
	}
	startListeners(&listeners);
	Server::getInstance()->deleteFileDescriptor(fd_);
}

int Configuration::openFile() {
	if (configPath_.length() < 6 ||
		configPath_.substr(configPath_.length() - 5).compare(stds(".conf")) != 0)
	{
		Log::get(logERROR) << "File should have the .conf extension" << std::endl;
		return (-1);
	}
	fd_ = open(configPath_.c_str(), O_RDONLY, 0664);
	if (fd_ < 0)
	{
		Log::get(logERROR) << " Unable to open: " << configPath_ << " : " << strerror(errno) << std::endl;
		return (fd_);
	}

	return (0);
}

void Configuration::shutdown_() {
	delete (Parsing::getInstance());
	delete (Server::getInstance());
	delete (Mime::getInstance());
	delete (Log::getInstance());
}


void Configuration::addListener(const Parsing::server &server, std::vector<Listener*> *listeners)
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

void Configuration::startListeners(const std::vector<Listener*> *listeners)
{
	for (uint64_t i = 0; i < listeners->size(); i++)
	{
		listeners->at(i)->ListenAndServe();
		Server::getInstance()->addFileDescriptor(listeners->at(i));
	}
}