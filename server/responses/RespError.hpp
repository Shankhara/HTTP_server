#ifndef WEBSERV_RESPERROR_HPP
#define WEBSERV_RESPERROR_HPP
#include "RespFile.hpp"

class RespError: public RespFile
{

private:
	size_t	payloadSize_;
	void 	setFilePath_();
	void 	writeDefaultErrorPage_(int statusCode);
	void 	writeStatusRelatedHeaders_();
	void 	writeHeaders_(size_t);

public:
	RespError(int, const Request &r, char [], unsigned int);
	~RespError();
	int readResponse();
	void build();
};

#endif //WEBSERV_RESPERROR_HPP
