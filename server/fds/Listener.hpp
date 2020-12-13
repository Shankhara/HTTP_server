#ifndef WEBSERV_LISTENER_HPP
#define WEBSERV_LISTENER_HPP

#include "FileDescriptor.hpp"
#include "../Server.hpp"
#include "Client.hpp"


class Listener: public FileDescriptor {
private:
	const uint32_t ip_;
	const int port_;
	const std::string name_;

public:
	Listener(uint32_t ip, int port, const std::string &name);
	virtual ~Listener();
	void	onEvent();
	void 	ListenAndServe();
	uint16_t htons_(uint16_t hostshort);
	void	onNewClient();
	int getPort() const;
};


#endif //WEBSERV_LISTENER_HPP
