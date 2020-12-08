#include "CGIExec.hpp"

std::vector<char *> envs_(16);

const std::string CGIExec::vars_[] = {
								  "AUTH_TYPE=",
								  "CONTENT_LENGTH=",
								  "GATEWAY_INTERFACE=",
								  "PATH_INFO=",
								  "PATH_TRANSLATED=",
								  "QUERY_STRING=",
								  "REMOTE_ADDR=",
								  "REMOTE_IDENT=",
								  "REMOTE_USER=",
								  "REQUEST_METHOD=",
								  "REQUEST_URI=",
								  "SCRIPT_NAME=",
								  "SERVER_NAME=",
								  "SERVER_PORT=",
								  "SERVER_PROTOCOL=",
								  "SERVER_SOFTWARE="
};

CGIExec::CGIExec(const Request &request): request_(request)
{
	setEnv_(AUTH_TYPE, "");
	setEnv_(CONTENT_LENGTH, "");
	setEnv_(GATEWAY_INTERFACE, "");
	setEnv_(PATH_INFO, "");
	setEnv_(PATH_TRANSLATED, "");
	setEnv_(QUERY_STRING, "");
	setEnv_(REMOTE_ADDR, "");
	setEnv_(REMOTE_IDENT, "");
	setEnv_(REMOTE_USER, "");
	setEnv_(REQUEST_METHOD, "");
	setEnv_(REQUEST_URI, "");
	setEnv_(SCRIPT_NAME, "");
	setEnv_(SERVER_NAME, "");
	setEnv_(SERVER_PORT, "");
	setEnv_(SERVER_PROTOCOL, "");
	setEnv_(SERVER_SOFTWARE, "webserver");
	envs_[15] = NULL;
}

CGIExec::~CGIExec() {}

void CGIExec::run() {
	pid_t cpid = fork();
	int status;

	if (cpid < 0)
	{
		Log().Get(logERROR) << "Unable to fork: " << strerror(errno);
		throw ("fork");
	}
	if (cpid == 0)
		exec_();
	else
	{
		if (waitpid(cpid, &status, 0) == -1)
		{
			Log().Get(logERROR) << "waitpid: " << strerror(errno);
			throw ("waitpid");
		}
	}
}

void CGIExec::exec_() {
	char * const cmd[] = { const_cast<char *>(cgiScript_.c_str()), NULL};
	int ret = execve(cgiScript_.c_str(), cmd, &envs_.data()[0]);
	if (ret == -1)
	{
		Log().Get(logERROR) << "execve " << strerror(errno);
		return ;
	}
}


void CGIExec::setEnv_(int name, std::string c) {
	std::string buf = vars_[name] + c;
	envs_[name] = const_cast<char *>(buf.c_str());
}
