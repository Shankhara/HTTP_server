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

#define BUFFER_SIZE 32768

class Client;

class CGISocket: public FileDescriptor {
private:
	Client 				&client_;
	std::string			resp_;
	pid_t  				pid_;
	bool 				httpStatus_;
	void 				parseCGIStatus(char buf[], int);
	void				appendContentLenght_();

public:
	static unsigned int instances;
	CGISocket(int, Client &);
	virtual		~CGISocket();
	void		onEvent();
	int			readCGIResponse();
	void		setPid(pid_t pid);
};

#endif
