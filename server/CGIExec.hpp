#ifndef WEBSERV_CGIEXEC_HPP
#define WEBSERV_CGIEXEC_HPP

#include "Log.hpp"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <wait.h>
#include <vector>
#include <map>
#include "Request.hpp"
#include "Server.hpp"
#include "fds/Client.hpp"
#include "fds/Listener.hpp"
#include "fds/CGISocket.hpp"
#include "responses/RespError.hpp"
class Client;

class CGIExec {
private:
	const Request				&request_;
	Client	 					&client_;
	int							stdoutFD_;
	int							stdinFD_;
	static const std::string	vars_[];
	std::vector<std::string>			envs_;
	enum e_envs {
		AUTH_TYPE,
		CONTENT_LENGTH,
		CONTENT_TYPE,
		GATEWAY_INTERFACE,
		PATH_INFO,
		PATH_TRANSLATED,
		QUERY_STRING,
		REMOTE_ADDR,
		REMOTE_IDENT,
		REMOTE_USER,
		REQUEST_URI,
		REQUEST_METHOD,
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
	void 						pipeSTDIN_(int pfd[2]);
	void 						dupSTDERR_();
	void 						build_(const std::string &);
	void 						write500();
	void 						setEnvFromHeader_(std::string name);

public:
	CGIExec(Client &);
	FileDescriptor				*run();
	virtual						~CGIExec();
};


#endif //WEBSERV_CGIEXEC_HPP
