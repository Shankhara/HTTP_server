#include "Listener.hpp"

void Listener::onEvent()
{
	Server::getInstance()->onClientConnect(fd_);
}

Listener::Listener(uint32_t ip, int port, const std::string &name): ip_(ip), port_(port), name_(name)
{
}

Listener::bind_() {
	struct sockaddr_in server;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ == -1)
	{
		Log().Get(logERROR) << "server:start -> error in socket()\n";
		exit(EXIT_FAILURE);
	}
	memset(&server, -1, sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ip_;
	server.sin_port = htons_(port_);
	if ((bind(fd_, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in bind() " << strerror(errno);
		exit(EXIT_FAILURE);
	}
	if (listen(fd_, FD_SETSIZE) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in listen() " << strerror(errno);
		exit(EXIT_FAILURE);
	}
}

uint16_t Listener::htons_(uint16_t hostshort)
{
	long ui = 0;

	ui |= (hostshort & 0xFF) << 8;
	ui |= (hostshort & 0xFF00) >> 8;
	return (ui);
}

Listener::~Listener() {}
