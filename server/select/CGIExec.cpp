#include "CGIExec.hpp"

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

CGIExec::~CGIExec() {}

CGIExec::CGIExec(const Request &r): request_(r) {
	envs_.reserve(16);
	setEnv_(AUTH_TYPE, "");
	setEnv_(CONTENT_LENGTH, "0");
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
	setEnv_(SERVER_SOFTWARE, "webserver/0.0.0");
	envs_[15] = 0;
}

void CGIExec::run()
{
	int pfd[2];
	pid_t cpid = fork();

	if (pipe(pfd))
	{
		Log().Get(logERROR) << "Unable to pipe: " << strerror(errno);
		throw ;
	}
	if (cpid < 0)
	{
		Log().Get(logERROR) << "Unable to fork: " << strerror(errno);
		throw ;
	}
	if (cpid == 0)
	{
		pipeStdout(pfd);
		Server::getInstance()->addFileDescriptor(*this);
		exec_();
		//TODO: remove FD from master set
	}
	else
	{
		// Since pfd[1] is going to be monitored by select lets not wait for a response from our CGI
		close(pfd[0]);
		close(pfd[1]);
	}
}

void CGIExec::exec_()
{
	char * const cmd[] = { const_cast<char *>(cgiScript_.c_str()), NULL};
	int ret = execve(cgiScript_.c_str(), cmd, &envs_.data()[0]);
	if (ret == -1)
	{
		Log().Get(logERROR) << "Unable to execve " << strerror(errno);
		throw;
	}
}


void	CGIExec::pipeStdout(int pfd[2])
{
	if (close(pfd[0]) == -1)
	{
		Log().Get(logERROR) << "Unable to close " << strerror(errno);
		throw ;
	}
	if (dup2(pfd[1], STDOUT_FILENO) == -1)
	{
		Log().Get(logERROR) << "Unable to dup2 " << strerror(errno);
		throw ;
	}
	if (close(pfd[1]) == -1)
	{
		Log().Get(logERROR) << "Unable to close " << strerror(errno);
		throw ;
	}
	fd_ = STDOUT_FILENO;
}

void CGIExec::setEnv_(int name, std::string c)
{
	std::string buf = vars_[name] + c;
	envs_[name] = const_cast<char *>(buf.c_str());
}


void CGIExec::onEvent() {
	//clientSendResponse
}