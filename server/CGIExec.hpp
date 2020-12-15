#ifndef WEBSERV_CGIEXEC_HPP
#define WEBSERV_CGIEXEC_HPP

#include "Logger.hpp"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <wait.h>
#include <vector>
#include "Request.hpp"
#include "Server.hpp"
#include "fds/Client.hpp"
#include "fds/Listener.hpp"
#include "fds/CGIResponse.hpp"

class Client;

class CGIExec {
private:
	int							stdoutFD_;
	static const std::string	vars_[];
	char *						envs_[19];
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
		SCRIPT_FILENAME,
		SCRIPT_NAME,
		SERVER_NAME,
		SERVER_PORT,
		SERVER_PROTOCOL,
		SERVER_SOFTWARE,
	};
	void						exec_(const std::string &, const std::string &);
	void 						setEnv_(int name, const std::string &);
	void 						pipeSTDOUT_(int pfd[2]);
	void 						dupSTDERR_();
	void 						build_(Request &, const std::string &, const std::string &);
	void						freeEnvs_();

public:
	CGIExec();
	FileDescriptor				*run(const std::string &, const std::string &, const std::string &, Client &);
	virtual						~CGIExec();
};


#endif //WEBSERV_CGIEXEC_HPP
