#pragma once

#include "../Request.hpp"
#include <errno.h>

#define READ_BUFFER_SIZE 16384

class Response
{
	protected:
	const Request		&req_;
	//std::string	msg_;
	int			statusCode_;
	char 		*buf_;
	unsigned int bufSize_;
	int			nbytes_;
	bool 		headersBuilt_;

	std::map<int, std::string> statusMap_;
	std::vector<std::string> headersToPut_;

	void append_(std::string );
	void append_(char [], int);

	public:
	Response(const Request &, char[], unsigned int bufSize);
	virtual ~Response();
	void appendStatusCode();
	void setBaseHeaders();
	void putHeaders();
	virtual int readResponse() = 0;
	void setHeaderContentType(std::string );
	void setHeaderContentLength(long);
	int error404();
};
