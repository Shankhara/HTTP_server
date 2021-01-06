#include "CGIExec.hpp"

const std::string CGIExec::vars_[] = {
								  "AUTH_TYPE=",
								  "CONTENT_LENGTH=",
								  "CONTENT_TYPE=",
								  "GATEWAY_INTERFACE=",
								  "PATH_INFO=",
								  "PATH_TRANSLATED=",
								  "QUERY_STRING=",
								  "REMOTE_ADDR=",
								  "REMOTE_IDENT=",
								  "REMOTE_USER=",
								  "REQUEST_URI=",
								  "REQUEST_METHOD=",
								  "SCRIPT_FILENAME=",
								  "SCRIPT_NAME=",
								  "SERVER_NAME=",
								  "SERVER_PORT=",
								  "SERVER_PROTOCOL=",
								  "SERVER_SOFTWARE="
};

CGIExec::~CGIExec() {}

CGIExec::CGIExec(Client &client): request_(client.getRequest()), client_(client) {
	envs_.push_back("REDIRECT_STATUS=200");
	const std::vector<std::string> &cHeaders = request_.getCGIHeaders();

	for (size_t i = 0; i < cHeaders.size(); i++)
		setEnvFromHeader_(cHeaders[i]);
}

void CGIExec::setEnvFromHeader_(std::string name)
{
	std::replace(name.begin(), name.end(), '-', '_');
	for (size_t i = 0; i < name.size(); i++)
	{
		if (name[i] == ':')
		{
			name[i] = '=';
			break ;
		}
		name[i] = std::toupper(name[i]);
	}
	envs_.push_back("HTTP_" + name);
}

void CGIExec::build_(const std::string &workDir) {
	setEnv_(CGIExec::AUTH_TYPE, request_.getHeaderAuth());
	setEnv_(CGIExec::CONTENT_LENGTH, ft_itoa(request_.getHeaderContentLength()));
	setEnv_(CGIExec::CONTENT_TYPE, request_.getHeaderContentType());
	setEnv_(CGIExec::GATEWAY_INTERFACE, "CGI/1.1");
	setEnv_(CGIExec::QUERY_STRING, request_.getQueryStr());
	setEnv_(CGIExec::PATH_INFO, request_.getOriginalReqTarget());
	setEnv_(CGIExec::PATH_TRANSLATED, workDir + request_.getReqTarget());
	setEnv_(CGIExec::REMOTE_ADDR, "");
	setEnv_(CGIExec::REMOTE_IDENT, "");
	setEnv_(CGIExec::REMOTE_USER, "");
	setEnv_(CGIExec::REQUEST_METHOD, request_.getMethod());
	setEnv_(CGIExec::REQUEST_URI, request_.getOriginalReqTarget());
	setEnv_(CGIExec::SCRIPT_FILENAME, workDir + request_.getReqTarget());
	setEnv_(CGIExec::SCRIPT_NAME, request_.getOriginalReqTarget());
	setEnv_(CGIExec::SERVER_NAME, request_.getHeaderHost());
	setEnv_(CGIExec::SERVER_PORT, ft_itoa(request_.getServer()->port));
	setEnv_(CGIExec::SERVER_PROTOCOL, "HTTP/1.1");
	setEnv_(CGIExec::SERVER_SOFTWARE, WEBSERV_ID);
}

FileDescriptor *CGIExec::run()
{
	int pipeOUT[2];
	int pipeIN[2];
	const Parsing::location *location = client_.getRequest().getLocation();

	Log::get(logDEBUG) << "CGI: " << location->cgi_path << " " << location->root << client_.getRequest().getReqTarget()  << std::endl;
	if (pipe(pipeOUT) == -1 || pipe(pipeIN) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << "Unable to pipe: " << strerror(errno) << std::endl;
		return (0);
	}
	CGISocket *pSocket = new CGISocket(pipeOUT[0], client_);
	build_(location->root);
	pid_t cpid = fork();
	if (cpid < 0)
	{
		delete pSocket;
		Log::get(logERROR) << __FUNCTION__  << "Unable to fork: " << strerror(errno) << std::endl;
		return (0);
	}
	if (cpid == 0)
	{
		pipeSTDOUT_(pipeOUT);
		pipeSTDIN_(pipeIN);
		if (chdir(location->root.c_str()) == -1)
		{
			Log::get(logERROR) << __FUNCTION__  << " Unable to chdir: " << strerror(errno) << " DIR: " << location->root << std::endl;
			write500();
		}
		dupSTDERR_();
		std::string target = client_.getRequest().getReqTarget();
		exec_(location->cgi_path, location->root + target);
	}
	else
	{
		close(pipeIN[0]);
		const std::string &body = client_.getRequest().getBody();
		if (!body.empty())
		{
			Log::get(logDEBUG) << __FUNCTION__  << " BODY SIZE:" << body.size() << std::endl;
			write(pipeIN[1], body.c_str(), body.size());
		}
		client_.flushRequest();
		pSocket->setPid(cpid);
		close(pipeOUT[1]);
		close(pipeIN[1]);
	}
	return (pSocket);
}

void CGIExec::exec_(const std::string &bin, const std::string &filename)
{
	static char * cmd[3];

	cmd[0] = const_cast<char *>(bin.c_str());
	cmd[1] = const_cast<char *>(filename.c_str());
	cmd[2] = 0;
	std::vector<char *> envs;
	envs.reserve(envs_.size() + 1);
	for (size_t i = 0; i < envs_.size(); ++i) {
		envs.push_back(const_cast<char *>(envs_[i].c_str()));
		//Log::get(logINFO) << "ENV: " << envs[i] << std::endl;
	}
	envs.push_back(0);
	if (execve(cmd[0], cmd, envs.data()) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << " Unable to execve " << strerror(errno) << std::endl;
		write500();
	}
}

void 	CGIExec::pipeSTDIN_(int pfd[2])
{
	if (close(pfd[1]) == -1)
		{
			Log::get(logERROR) << __FUNCTION__  << " Unable to close " << strerror(errno) << std::endl;
			write500();
		}
	if (dup2(pfd[0], STDIN_FILENO) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << "Unable to dup2 " << strerror(errno) << std::endl;
		write500();
	}
	if (close(pfd[0]) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << "Unable to close " << strerror(errno) << std::endl;
		write500();
	}
	stdinFD_ = STDIN_FILENO;
}

void	CGIExec::pipeSTDOUT_(int pfd[2])
{
	if (close(pfd[0]) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << " Unable to close " << strerror(errno) << std::endl;
		write500();
	}
	if (dup2(pfd[1], STDOUT_FILENO) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << "Unable to dup2 " << strerror(errno) << std::endl;
		write500();
	}
	if (close(pfd[1]) == -1)
	{
		Log::get(logERROR) << __FUNCTION__  << "Unable to close " << strerror(errno) << std::endl;
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
	envs_.push_back(vars_[name] + c);
}

void CGIExec::write500() {
	static char buf[2048];
	int nbytes;

	RespError err(500, request_, buf, 2048);
	while ((nbytes = err.readResponse()) > 0)
	{
		write(STDOUT_FILENO, buf, nbytes);
	}
	exit(EXIT_FAILURE);
}

