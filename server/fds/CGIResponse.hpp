#ifndef WEBSERV_CGIRESPONSE_HPP
#define WEBSERV_CGIRESPONSE_HPP

#include "FileDescriptor.hpp"
#include "Client.hpp"
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include "../Logger.hpp"

class CGIResponse: public FileDescriptor {
private:
	Client &client_;
public:
	CGIResponse(int, Client &);
	virtual ~CGIResponse();
	void onEvent();
};


#endif //WEBSERV_CGIRESPONSE_HPP
