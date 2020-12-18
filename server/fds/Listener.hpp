#ifndef WEBSERV_LISTENER_HPP
#define WEBSERV_LISTENER_HPP

#include "FileDescriptor.hpp"
#include "../Server.hpp"
#include "Client.hpp"
#include "../parsing/Parsing.hpp"


class Listener: public FileDescriptor {
private:
	uint32_t						ip_;
	unsigned int					port_;
	std::vector<Parsing::server>	servers_;

public:
	Listener();
	virtual 			~Listener();
	void				onEvent();
	void 				addServer(const Parsing::server &);
	void 				ListenAndServe();
	uint16_t 			htons_(uint16_t hostshort);
	void				onNewClient();
	unsigned int 		getPort() const;
};


#endif //WEBSERV_LISTENER_HPP
