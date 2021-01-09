#pragma once

#include "../Request.hpp"
#include "../Mime.hpp"
#include <errno.h>

class Response
{
	protected:
	const Request				&req_;
	char 						*buf_;
	unsigned int				bufSize_;
	int							nbytes_;
	bool 						headersBuilt_;
	int 						statusCode_;
	std::vector<std::string> headers_;
	static std::map<int, std::string> statusMap_;

    void initHeaders ();
	void writeContentType_(std::string);
	void writeContentLength_(long);
    void writeAllow_();
	void writeHeadersEnd_();
	void writeThisHeader_(const std::string&, const std::string&);

	virtual void 	append_(const std::string &);
	virtual void 	append_(const char [], unsigned int);

	public:
	Response(const Request &, char[], unsigned int bufSize);
	virtual			~Response();
	int				writeErrorPage(int);
	void 			writeErrorBody(int);
	void 			appendHeaders(int, std::string, unsigned int);
	virtual int		readResponse() = 0;
	virtual void 	build() = 0;
	int 			getStatusCode() const;

};
