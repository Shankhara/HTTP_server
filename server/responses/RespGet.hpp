#ifndef WEBSERV_GETMETHOD_HPP
#define WEBSERV_GETMETHOD_HPP
#include "Response.hpp"
#include "../Logger.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class RespGet: public Response {
private:
	int fd_;
public:
	RespGet(const Request &r, char[], unsigned int);
	~RespGet();
	int readResponse();

};


#endif //WEBSERV_GETMETHOD_HPP
