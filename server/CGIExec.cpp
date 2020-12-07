#include "CGIExec.hpp"

CGIExec::CGIExec(const std::string &scriptPath): cgiScript_(scriptPath)
{
	envs_.push_back(const_cast<char*>("AUTH_TYPE="));
	envs_.push_back(const_cast<char*>("CONTENT_LENGTH=0"));
	envs_.push_back(const_cast<char*>("CONTENT_TYPE="));
	envs_.push_back(const_cast<char*>("GATEWAY_INTERFACE="));
	envs_.push_back(const_cast<char*>("PATH_INFO=/"));
	envs_.push_back(const_cast<char*>("PATH_TRANSLATED=/"));
	envs_.push_back(const_cast<char*>("QUERY_STRING="));
	envs_.push_back(const_cast<char*>("PATH_TRANSLATED=/"));
	envs_.push_back(const_cast<char*>("REMOTE_ADDR=127.0.0.1"));
	envs_.push_back(const_cast<char*>("REMOTE_IDENT="));
	envs_.push_back(const_cast<char*>("REMOTE_USER="));
	envs_.push_back(const_cast<char*>("REQUEST_METHOD=GET"));
	envs_.push_back(const_cast<char*>("REQUEST_URI="));
	envs_.push_back(const_cast<char*>("SCRIPT_NAME=/cgi/test.cgi"));
	envs_.push_back(const_cast<char*>("SERVER_NAME=webserver"));
	envs_.push_back(const_cast<char*>("SERVER_PORT=8080"));
	envs_.push_back(const_cast<char*>("SERVER_PROTOCOL=HTTP1.1"));
	envs_.push_back(const_cast<char*>("SERVER_SOFTWARE=Webserver/0.0.0"));
	envs_.push_back(NULL);
}

CGIExec::~CGIExec() {

}

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
