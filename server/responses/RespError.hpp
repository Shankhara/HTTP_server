#ifndef WEBSERV_RESPERROR_HPP
#define WEBSERV_RESPERROR_HPP
#include "Response.hpp"

class RespError: public Response
{

private:
	void	writeErrorBody_(int);
	std::string getErrorPage_(int statusCode);

public:
	RespError(int, const Request &r, char [], unsigned int);
	~RespError();
	int readResponse();
	void build();
};

#endif //WEBSERV_RESPERROR_HPP
