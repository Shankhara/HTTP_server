#ifndef WEBSERV_RESPCGI_HPP
#define WEBSERV_RESPCGI_HPP
#include "Response.hpp"
#include "../fds/Client.hpp"
#include "RespError.hpp"

class Client;

class RespCGI: public Response {

private:
	int 	fd_;
	std::string resp_;
	size_t respCursor_;
	void parseCGIStatus_(int nbytes);
	void setStatusCode_(const std::string &);

public:
	RespCGI(Client &, int fd);
	~RespCGI();
	int readResponse();
	void build();
};


#endif //WEBSERV_RESPCGI_HPP
