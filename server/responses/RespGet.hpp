#ifndef WEBSERV_GETMETHOD_HPP
#define WEBSERV_GETMETHOD_HPP
#include "Response.hpp"
#include "../Logger.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class RespGet: public Response {
public:
	RespGet(const Request &r);
	~RespGet();
	void build();
private:

};


#endif //WEBSERV_GETMETHOD_HPP
