#ifndef WEBSERV_CGIEXEC_HPP
#define WEBSERV_CGIEXEC_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <wait.h>
#include <vector>
#include "Request.hpp"

class CGIExec {
private:
	const Request				request_;
	static const std::string	vars_[];
	std::vector<char *>	envs_;
	static CGIExec				*instance_;
	std::string 				cgiScript_;
	enum e_envs {
		AUTH_TYPE,
		CONTENT_LENGTH,
		GATEWAY_INTERFACE,
		PATH_INFO,
		QUERY_STRING,
		PATH_TRANSLATED,
		REMOTE_ADDR,
		REMOTE_IDENT,
		REMOTE_USER,
		REQUEST_METHOD,
		REQUEST_URI,
		SCRIPT_NAME,
		SERVER_NAME,
		SERVER_PORT,
		SERVER_PROTOCOL,
		SERVER_SOFTWARE
	};
	void							exec_();
	void 						setEnv_(int name, std::string c);
	void 						pipeStdout(int pfd[2]);
	CGIExec();

public:
	void 						build(const Request &);
	static CGIExec				*getInstance();
	void 						run();
	~CGIExec();
};


#endif //WEBSERV_CGIEXEC_HPP
