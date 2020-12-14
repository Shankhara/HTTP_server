#ifndef WEBSERV_CGIRESPONSE_HPP
#define WEBSERV_CGIRESPONSE_HPP

#include "FileDescriptor.hpp"
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <wait.h>
#include "../Server.hpp"
#include "../Logger.hpp"
#include "Client.hpp"

class Client;

class CGIResponse: public FileDescriptor {
private:
	Client &client_;
	pid_t  pid_;

public:
	CGIResponse(int, Client &);
	virtual		~CGIResponse();
	void		onEvent();
	int			readPipe();
	pid_t		getPid() const;
	void		setPid(pid_t pid);
};

#endif
