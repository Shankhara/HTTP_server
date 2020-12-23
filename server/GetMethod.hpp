#ifndef WEBSERV_GETMETHOD_HPP
#define WEBSERV_GETMETHOD_HPP
#include "Response.hpp"
#include "Logger.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class GetMethod: public Response {
public:
	GetMethod(const Request &r);
	~GetMethod();
	void exec();
private:

};


#endif //WEBSERV_GETMETHOD_HPP
