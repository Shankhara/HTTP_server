#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): Response(r, buf, bufSize)
{
	statusCode_ = statusCode;
}

RespError::~RespError() {}

int RespError::readResponse()
{
	nbytes_ = 0;
	
	if (headersBuilt_ == true)
		return 0;

	initHeaders();
    if (statusCode_ == 401)
		writeThisHeader_("WWW-Authenticate", "Basic realm=\"simple\"");
	if (statusCode_ == 405)
	    writeAllow_();
	if (statusCode_ == 406)
        writeThisHeader_("Accept-charset", "utf-8");
    if (statusCode_ == 503)
	    writeThisHeader_("Retry-After", "10");

	writeErrorBody_(statusCode_);
	return nbytes_;
}

void RespError::writeErrorBody_(int statusCode)
{
	std::string path = getErrorPage_(statusCode);
	std::string body;

	if (!path.empty())
		body = "Suppose to be the file";
	else
	{
		body = 		"<html>"
					  "<head><title>" + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</title></head>"
																							 "<body bgcolor=\"white\">"
																							 "<center><h1>"+ ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</h1></center>"
																																								  "<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
																																																		   "</body>"
																																																		   "</html>";
	}
	writeThisHeader_("Content-Type", "text/html");
	writeContentLength_(body.size());
	writeHeadersEnd_();
	if (req_.getMethod() == "HEAD")
		return ;
	append_(body);
}

std::string	RespError::getErrorPage_(int statusCode)
{
	const Parsing::server *server = req_.getServer();
	if (server == 0)
		return "";
	for (size_t i = 0; i < server->error_pages.size(); i++)
		if (server->error_pages[i].first == statusCode)
			return server->error_pages[i].second;
	return "";
}

void RespError::build() {}

