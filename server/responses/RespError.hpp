#ifndef WEBSERV_RESPERROR_HPP
#define WEBSERV_RESPERROR_HPP
#include "Response.hpp"

class RespError: public Response
{
	private:
	int fd_;
	int statusCode_;
	
	public:
	RespError(int, const Request &r, char [], unsigned int);
	~RespError();
	int readResponse();
};

#endif //WEBSERV_RESPERROR_HPP
