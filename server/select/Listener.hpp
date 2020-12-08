#ifndef WEBSERV_LISTENER_HPP
#define WEBSERV_LISTENER_HPP

#include "FileDescriptor.hpp"
#include "../Server.hpp"


class Listener: public FileDescriptor {
private:
	const uint32_t ip_;
	const int port_;
	const std::string name_;
public:
	Listener(int fd, uint32_t ip, int port, const std::string &name);
	Listener(int);
	virtual ~Listener();
	void	onEvent();
	void 	bind_();
	uint16_t htons_(uint16_t hostshort);
};


#endif //WEBSERV_LISTENER_HPP
