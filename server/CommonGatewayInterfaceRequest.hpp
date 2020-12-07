#ifndef WEBSERV_COMMONGATEWAYINTERFACEREQUEST_HPP
#define WEBSERV_COMMONGATEWAYINTERFACEREQUEST_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <wait.h>
#include <vector>

class CommonGatewayInterfaceRequest {
private:
	std::string 				cgiScript_;
	std::vector<char *>			envs_;
	void						exec_();

public:
	CommonGatewayInterfaceRequest(const std::string &);
	~CommonGatewayInterfaceRequest();
	void run() throw();
};


#endif //WEBSERV_COMMONGATEWAYINTERFACEREQUEST_HPP
