#ifndef WEBSERV_CGISOCKET_HPP
#define WEBSERV_CGISOCKET_HPP

#include "FileDescriptor.hpp"
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>
#include "../Server.hpp"
#include "../Log.hpp"
#include "Client.hpp"
#include "../responses/Response.hpp"
#include "../responses/RespCGI.hpp"

class Client;

class CGISocket: public FileDescriptor {
private:
	Client 				&client_;
	pid_t  				pid_;

public:
	static unsigned int instances;
	CGISocket(int, Client &);
	virtual		~CGISocket();
	void		onEvent();
	void 		onTimeout(){};
	void		setPid(pid_t pid);
};

#endif
