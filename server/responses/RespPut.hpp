#ifndef WEBSERV_RESPPUT_HPP
#define WEBSERV_RESPPUT_HPP
#include "Response.hpp"

class RespPut: public Response {
private:
public:
	RespPut(const Request &r, char[], unsigned int);
	~RespPut();
};


#endif //WEBSERV_RESPPUT_HPP
