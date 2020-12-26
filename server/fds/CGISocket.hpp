#ifndef WEBSERV_CGISOCKET_HPP
#define WEBSERV_CGISOCKET_HPP

#include "FileDescriptor.hpp"
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>
#include "../Server.hpp"
#include "../Logger.hpp"
#include "Client.hpp"
#include "../responses/Response.hpp"

#define BUFFER_SIZE 32768

class Client;

class CGISocket: public FileDescriptor {
private:
	Client 				&client_;
	pid_t  				pid_;
	bool 				httpStatus_;
	void 				parseCGIStatus(char buf[], int);

public:
	static unsigned int instances;
	CGISocket(int, Client &);
	virtual		~CGISocket();
	void		onEvent();
	int			pipeToClient();
	void		setPid(pid_t pid);
};

#endif