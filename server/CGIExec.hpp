#ifndef WEBSERV_CGIEXEC_HPP
#define WEBSERV_CGIEXEC_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <wait.h>
#include <vector>

class CGIExec {
private:
	std::string 				cgiScript_;
	std::vector<char *>			envs_;
	void						exec_();

public:
	CGIExec(const std::string &);
	~CGIExec();
	void run();
};


#endif //WEBSERV_CGIEXEC_HPP
