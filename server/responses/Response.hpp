#pragma once

#include "../Request.hpp"
#include "../Mime.hpp"
#include <errno.h>

class Response
{
	protected:
	const Request						&req_;
	char 								*buf_;
	int									nbytes_;
	int									fd_;
	int 								statusCode_;
	bool 								headersBuilt_;
	unsigned int						bufSize_;
	std::string 						filePath_;
	std::string 						payload_;
	static std::map<int, std::string>	statusMap_;

	void						writeStatusLine_(int);
	void						writeBaseHeaders_();
	void						writeContentType_(std::string);
	void						writeContentLength_(long);
	void 						writeHeadersEnd_();
	void 						writeThisHeader_(std::string, std::string);
	void 						writeStatusLine_();
	void 						writeContentType_();

	virtual void append_(std::string);
	virtual void append_(char [], unsigned int);

	public:
	Response(const Request &, char[], unsigned int bufSize);
	virtual			~Response();
	int				writeErrorPage(int);
	void 			writeErrorBody(int);
	void 			appendHeaders(int, std::string, unsigned int);
	virtual int		readResponse() = 0;

	void setFilePath();
};
