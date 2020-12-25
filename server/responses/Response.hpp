#pragma once

#include "../Request.hpp"
#include <errno.h>

class Response
{
	protected:
	const Request				&req_;
	char 						*buf_;
	unsigned int				bufSize_;
	int							nbytes_;
	bool 						headersBuilt_;

	std::map<int, std::string> statusMap_; //TODO: static instead of allocating this every request

	void append_(std::string );
	void append_(char [], unsigned int);

	public:
	Response(const Request &, char[], unsigned int bufSize);
	virtual			~Response();
	void			appendStatusCode(int);
	void			appendBaseHeaders();
	void			setHeaderContentType(std::string );
	void			setHeaderContentLength(long);
	int				writeErrorPage(int);
	void 			appendHeadersEnd();
	void 			appendHeaders(std::string, unsigned int);
	virtual int		readResponse() = 0;
};
