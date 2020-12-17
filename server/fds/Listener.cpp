#include "Listener.hpp"

Listener::Listener(): ip_(0), port_(0) {}

Listener::~Listener(){
	if (fd_ > 0)
		close(fd_);
}

void Listener::onEvent()
{
	onNewClient();
}

void Listener::onNewClient()
{
	socklen_t 				addrlen;
	struct sockaddr_storage remoteaddr;
	int						newfd;

	addrlen = sizeof(remoteaddr);
	if ((newfd = accept(fd_, (struct sockaddr *)(&remoteaddr), &addrlen)) == -1)
	{
		Log().Get(logERROR) << "server::onClientConnect " << strerror(errno);
		exit(8);
	}
	Client *client = new Client(newfd, servers_[0]);
	client->setAddr(remoteaddr);
	Server::getInstance()->addFileDescriptor(client);
}

void Listener::ListenAndServe() {
	struct sockaddr_in server;
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ == -1)
	{
		Log().Get(logERROR) << "server:start -> error in socket()\n";
		delete Server::getInstance();
		exit(EXIT_FAILURE);
	}
	int enable = 1;
	if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		Log().Get(logERROR) << __FUNCTION__  << " setsockopt failed" << strerror(errno);
	memset(&server, 0, sizeof(sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = ip_;
	server.sin_port = htons_(port_);
	if ((bind(fd_, (struct sockaddr *)&server, sizeof(struct sockaddr))) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in bind() " << strerror(errno);
		delete Server::getInstance();
		exit(EXIT_FAILURE) ;
	}
	if (listen(fd_, FD_SETSIZE) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in listen() " << strerror(errno);
		delete Server::getInstance();
		exit(EXIT_FAILURE) ;
	}
}

uint16_t Listener::htons_(uint16_t hostshort)
{
	long ui = 0;

	ui |= (hostshort & 0xFF) << 8;
	ui |= (hostshort & 0xFF00) >> 8;
	return (ui);
}


unsigned int Listener::getPort() const {
	return port_;
}

void Listener::addServer(const Parsing::server &s) {
	in_addr_t  host = inet_addr(s.host.c_str());
	if (host == INADDR_NONE)
	{
		Log().Get(logERROR) << __FUNCTION__  << " Unable to add: " << s.host;
		return;
	}
	if (ip_ == 0 && port_ == 0)
	{
		ip_ = host;
		port_ = s.port;
	}
	else if (host != ip_ || port_ != s.port)
	{
		Log().Get(logERROR) << __FUNCTION__  << " this host:port doesnt belong to this listener: " << s.host << ":" << s.port;
		return;
	}
	Log().Get(logINFO) << s.names[0] << " started on port " << s.host << ":" << s.port << " (maxconn: " << FD_SETSIZE << ")";
	servers_.push_back(s);
}

