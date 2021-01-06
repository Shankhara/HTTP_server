#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <cstring>
#include <sys/socket.h>
#include "../Log.hpp"
#include "../Server.hpp"
#include "FileDescriptor.hpp"
#include "../Request.hpp"
#include "../CGIExec.hpp"
#include "Listener.hpp"
#include "../parsing/Parsing.hpp"
#include "../responses/RespGet.hpp"
#include "../responses/RespHead.hpp"
#include "../responses/RespError.hpp"
#include "../responses/RespPut.hpp"
#include "../responses/Response.hpp"
#include "../responses/RespPost.hpp"
#include "../responses/RespDelete.hpp"
#include "../responses/RespOptions.hpp"
#include "../responses/RespTrace.hpp"

#define MAX_CGI_FORKS 20
#define CLIENT_BUFFER_SIZE	65536

class Listener;

class Client: public FileDescriptor {

private:
	FileDescriptor						*CGIResponse_;
	Response							*resp_;
	Request								*request_;
	static char 						buf_[CLIENT_BUFFER_SIZE];
	void 								doResponse_();
	void 								sendResponse_(Response *);
	void 								doStaticFile_();
	void 								doCGI_();
	void								responseFactory_();
	bool 								isFileCGI_(const Parsing::location *, std::string);

public:
	Client(int, const Listener &);
	virtual 						~Client();
	void 							constructRequest(char [], int);
	void 							onEvent();
	Request							&getRequest();
	void 							flushRequest();
};

#endif //WEBSERV_CLIENT_HPP
