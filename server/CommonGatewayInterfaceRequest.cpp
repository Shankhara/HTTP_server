#include "CommonGatewayInterfaceRequest.hpp"

CommonGatewayInterfaceRequest::CommonGatewayInterfaceRequest(const std::string &scriptPath): cgiScript_(scriptPath)
{
	envs_.push_back(const_cast<char *>(scriptPath.c_str()));
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

CommonGatewayInterfaceRequest::~CommonGatewayInterfaceRequest() {

}

void CommonGatewayInterfaceRequest::run() throw() {
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
		if (waitpid(cpid, &status, NULL) == -1)
		{
			Log().Get(logERROR) << "waitpid: " << strerror(errno);
			throw ("waitpid");
		}
	}
}

void CommonGatewayInterfaceRequest::exec_() {
	int ret = execve(envs_.data()[0], NULL, &envs_.data()[0]);
	if (ret == -1)
	{
		Log().Get(logERROR) << "execve " << strerror(errno);
		return ;
	}
}
