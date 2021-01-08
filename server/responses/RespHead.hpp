#ifndef WEBSERV_RESPHEAD_HPP
#define WEBSERV_RESPHEAD_HPP

#include "RespGet.hpp"

class RespHead: public RespGet {
private:
	void append_(std::string);
	void append_(const char [], unsigned int);
public:
	RespHead(const Request &r, char [], unsigned int);
	~RespHead();
	virtual int readResponse();
	virtual void build();
};


#endif //WEBSERV_RESPHEAD_HPP
