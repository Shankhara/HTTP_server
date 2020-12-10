#ifndef WEBSERV_CGIEXEC_HPP
#define WEBSERV_CGIEXEC_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <wait.h>
#include <vector>
#include "RequestMock.hpp"
#include "Server.hpp"
#include "fds/CGIResponse.hpp"

class CGIExec {
private:
	int							stdoutFD_;
	static const std::string	vars_[];
	char *						envs_[17];
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
	void						exec_(const std::string &);
	void 						setEnv_(int name, std::string c);
	void 						pipeStdout(int pfd[2]);
	void 						build_(const RequestMock &);
	void						freeEnvs_();

public:
	CGIExec();
	CGIResponse					*run(const std::string &, RequestMock &);
	virtual						~CGIExec();
};


#endif //WEBSERV_CGIEXEC_HPP
