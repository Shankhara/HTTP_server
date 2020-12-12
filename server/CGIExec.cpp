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
								  "SCRIPT_FILENAME=",
								  "SCRIPT_NAME=",
								  "SERVER_NAME=",
								  "SERVER_PORT=",
								  "SERVER_PROTOCOL=",
								  "SERVER_SOFTWARE="
};

CGIExec::~CGIExec() {
	free(envs_[17]);
}

CGIExec::CGIExec() {
	setEnv_(17, "REDIRECT_STATUS=200");
	envs_[18] = 0;
}

void CGIExec::build_(const RequestMock &request, const std::string &workDir, const std::string &filename) {
	setEnv_(AUTH_TYPE, "");
	setEnv_(CONTENT_LENGTH, request.getHeaderContentLength());
	setEnv_(GATEWAY_INTERFACE, "CGI/1.1");
	setEnv_(PATH_INFO, "");
	setEnv_(PATH_TRANSLATED, "");
	setEnv_(QUERY_STRING, "");
	setEnv_(REMOTE_ADDR, "");
	setEnv_(REMOTE_IDENT, "");
	setEnv_(REMOTE_USER, "");
	setEnv_(REQUEST_METHOD, request.getRequestMethod());
	setEnv_(REQUEST_URI, "");
	setEnv_(SCRIPT_FILENAME, workDir + filename);
	setEnv_(SCRIPT_NAME, filename);
	setEnv_(SERVER_NAME, "");
	setEnv_(SERVER_PORT, itoa_(dynamic_cast<Listener &>(request.getClient().getListener()).getPort()));
	setEnv_(SERVER_PROTOCOL, "");
	setEnv_(SERVER_SOFTWARE, "webserver/0.0.0");
}

CGIResponse *CGIExec::run(const std::string &cgiBin, const std::string &workingDir,
						  const std::string &filename, RequestMock &request)
{
	int pfd[2];

	Log().Get(logDEBUG) << "WD: " << workingDir << "BIN: " << cgiBin << " " << filename;
	if (pipe(pfd))
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to pipe: " << strerror(errno);
		return (0);
	}
	CGIResponse *response = new CGIResponse(pfd[0], request.getClient());
	pid_t cpid = fork();
	if (cpid < 0)
	{
		delete response;
		Log().Get(logERROR) << __FUNCTION__  << "Unable to fork: " << strerror(errno);
		return (0);
	}
	if (cpid == 0)
	{
		if (chdir(workingDir.c_str()) == -1)
		{
			Log().Get(logERROR) << __FUNCTION__  << " Unable to chdir: " << strerror(errno);
			exit(255);
		}
		pipeSTDOUT_(pfd);
		build_(request, workingDir, filename);
		dupSTDERR_();
		exec_(cgiBin, filename);
		close(STDOUT_FILENO);
		exit(0);
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
		Log().Get(logERROR) << __FUNCTION__  << " Unable to execve " << strerror(errno);
	}
}


void	CGIExec::pipeSTDOUT_(int pfd[2])
{
	if (close(pfd[0]) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << " Unable to close " << strerror(errno);
		exit(EXIT_FAILURE);
	}
	if (dup2(pfd[1], STDOUT_FILENO) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to dup2 " << strerror(errno);
		exit(EXIT_FAILURE);
	}
	if (close(pfd[1]) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to close " << strerror(errno);
		exit(EXIT_FAILURE);
	}
	stdoutFD_ = STDOUT_FILENO;
}

void	CGIExec::dupSTDERR_()
{
	int fd;
	if ((fd = open("/dev/null", O_RDONLY)) == -1)
		return ;
	dup2(fd, STDERR_FILENO);
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

std::string CGIExec::itoa_(int nb)
{
	std::string ss;
	while(nb)
	{
		int x = nb%10;
		nb /= 10;
		ss+= 48 + x;
	}
	return ss;
}

