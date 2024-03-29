#ifndef WEBSERV_RESPEXCEPTION_HPP
#define WEBSERV_RESPEXCEPTION_HPP
#include "exception"
#include "string"
#include "../Utils.hpp"

class RespException: public std::exception {

private:
	const int statusCode_;
	std::string location_;

public:
	RespException(int);
	virtual const char *what() const throw();
	int getStatusCode() const;
	const std::string &getLocation() const;
	virtual ~RespException() throw();
};


#endif //WEBSERV_RESPEXCEPTION_HPP
