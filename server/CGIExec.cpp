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

CGIExec::~CGIExec() {
	free(envs_[16]);
}

CGIExec::CGIExec() {
	setEnv_(16, "REDIRECT_STATUS=200");
	envs_[17] = 0;
}

void CGIExec::build_(const RequestMock &request) {
	setEnv_(AUTH_TYPE, "");
	setEnv_(CONTENT_LENGTH, request.getHeaderContentLength());
	setEnv_(GATEWAY_INTERFACE, "");
	setEnv_(PATH_INFO, "");
	setEnv_(PATH_TRANSLATED, "");
	setEnv_(QUERY_STRING, "");
	setEnv_(REMOTE_ADDR, "");
	setEnv_(REMOTE_IDENT, "");
	setEnv_(REMOTE_USER, "");
	setEnv_(REQUEST_METHOD, request.getRequestMethod());
	setEnv_(REQUEST_URI, "");
	setEnv_(SCRIPT_NAME, "");
	setEnv_(SERVER_NAME, "");
	setEnv_(SERVER_PORT, "");
	setEnv_(SERVER_PROTOCOL, "");
	setEnv_(SERVER_SOFTWARE, "webserver/0.0.0");
}

CGIResponse *CGIExec::run(const std::string &bin, const std::string &filename, RequestMock &request)
{
	int pfd[2];

	if (pipe(pfd))
	{
		Log().Get(logERROR) << "Unable to pipe: " << strerror(errno);
		return (0);
	}
	CGIResponse *response = new CGIResponse(pfd[0], request.getClient());
	pid_t cpid = fork();
	if (cpid < 0)
	{
		delete response;
		Log().Get(logERROR) << "Unable to fork: " << strerror(errno);
		return (0);
	}
	if (cpid == 0)
	{
		pipeStdout(pfd);
		build_(request);
		exec_(bin, filename);
		close(STDOUT_FILENO);
	}
	else
	{
		response->setPid(cpid);
		close(pfd[1]);
	}
	return (response);
}

void CGIExec::exec_(const std::string &bin, const std::string &filename)
{
	static char * cmd[3];
	cmd[0] = const_cast<char *>(bin.c_str());
	cmd[1] = const_cast<char *>(filename.c_str());
	cmd[2] = 0;
	int ret = execve(cmd[0], cmd, envs_);
	freeEnvs_();
	if (ret == -1)
	{
		Log().Get(logERROR) << "Unable to execve " << strerror(errno);
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
	stdoutFD_ = STDOUT_FILENO;
}

void CGIExec::setEnv_(int name, std::string c)
{
	//TODO: concat instead allocating tmp buf
	std::string buf = vars_[name] + c;
	envs_[name] = (char*)malloc((buf.length() + 1) * sizeof(char));
	if (envs_[name] == 0)
		return ;
	unsigned long i;
	for (i = 0; i < buf.length(); i++)
	{
		envs_[name][i] = buf[i];
	}
	envs_[name][i] = '\0';
}

void CGIExec::freeEnvs_()
{
	for (int i = 0; i < 16; i++)
		free(envs_[i]);
}

