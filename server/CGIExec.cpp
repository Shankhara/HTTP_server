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
	free(envs_[18]);
}

CGIExec::CGIExec() {
	setEnv_(18, "REDIRECT_STATUS=200");
	envs_[19] = 0;
}

void CGIExec::build_(Request &request, const std::string &workDir, const std::string &filename) {
	setEnv_(CGIExec::AUTH_TYPE, request.getHeaderAuth());
	setEnv_(CGIExec::CONTENT_LENGTH, ft_itoa(request.getHeaderContentLength()));
	setEnv_(CGIExec::CONTENT_TYPE, request.getHeaderContentType()); //TODO ???
	setEnv_(CGIExec::GATEWAY_INTERFACE, "CGI/1.1");
	setEnv_(CGIExec::QUERY_STRING, ""); //TODO: getQueryString?
	setEnv_(CGIExec::PATH_INFO, request.getReqTarget());
	setEnv_(CGIExec::PATH_TRANSLATED, workDir + request.getReqTarget());
	setEnv_(CGIExec::REMOTE_ADDR, "127.0.0.1");
	setEnv_(CGIExec::REMOTE_IDENT, ""); // absolute nightmare.
	setEnv_(CGIExec::REMOTE_USER, ""); // same.
	setEnv_(CGIExec::REQUEST_METHOD, request.getMethod());
	setEnv_(CGIExec::REQUEST_URI, request.getReqTarget());
	setEnv_(CGIExec::SCRIPT_FILENAME, workDir + filename);
	setEnv_(CGIExec::SCRIPT_NAME, filename);
	setEnv_(CGIExec::SERVER_NAME, request.getHeaderHost());
	setEnv_(CGIExec::SERVER_PORT, ft_itoa(request.getServer().port));
	setEnv_(CGIExec::SERVER_PROTOCOL, "HTTP/1.1");
	setEnv_(CGIExec::SERVER_SOFTWARE, "webserv/0.0.0");
}

FileDescriptor *CGIExec::run(const std::string &cgiBin, const std::string &workingDir,
						  const std::string &filename, Client &client)
{
	int pfd[2];

	Log().Get(logDEBUG) << "CGI: " << cgiBin << " " << workingDir << filename ;
	if (pipe(pfd))
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to pipe: " << strerror(errno);
		return (0);
	}
	CGIResponse *response = new CGIResponse(pfd[0], client);
	build_(client.getRequest(), workingDir, filename);
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
			Log().Get(logERROR) << __FUNCTION__  << " Unable to chdir: " << strerror(errno) << " DIR: " << workingDir;
			exit(EXIT_FAILURE);
		}
		pipeSTDOUT_(pfd);
		dupSTDERR_();
		exec_(cgiBin, workingDir + filename);
		close(STDOUT_FILENO);
		exit(EXIT_FAILURE);
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
	if (execve(cmd[0], cmd, envs_) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << " Unable to execve " << strerror(errno);
		write500();
	}
}


void	CGIExec::pipeSTDOUT_(int pfd[2])
{
	if (close(pfd[0]) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << " Unable to close " << strerror(errno);
		write500();
	}
	if (dup2(pfd[1], STDOUT_FILENO) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to dup2 " << strerror(errno);
		write500();
	}
	if (close(pfd[1]) == -1)
	{
		Log().Get(logERROR) << __FUNCTION__  << "Unable to close " << strerror(errno);
		write500();
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

void CGIExec::setEnv_(int name, const std::string &c)
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
	Log().Get(logDEBUG) << envs_[name];
}

void CGIExec::freeEnvs_()
{
	for (int i = 0; i < 18; i++)
		free(envs_[i]);
}

void CGIExec::write500() {
	write(STDOUT_FILENO, "Status: 500 Internal Server Error\r\n", 36);
	exit(EXIT_FAILURE);
}

