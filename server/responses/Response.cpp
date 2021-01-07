#include "Response.hpp"

std::map<int, std::string> Response::statusMap_;

Response::Response(const Request & r, char buf[], unsigned int bufSize) : \
	req_(r), buf_(buf), bufSize_(bufSize - 1)
{
	nbytes_ = 0;
	headersBuilt_ = false;
	statusCode_ = 200;

	if (statusMap_.size() == 0)
	{
		statusMap_[200] = "OK";
		statusMap_[201] = "Created";
		statusMap_[202] = "Accepted";
		statusMap_[204] = "No Content";
		statusMap_[400] = "Bad Request";
		statusMap_[401] = "Unauthorized";
		statusMap_[403] = "Forbidden";
		statusMap_[404] = "Not Found";
		statusMap_[405] = "Method Not Allowed";
		statusMap_[413] = "Request Entity Too Large";
		statusMap_[414] = "Request-URI Too Long";
		statusMap_[500] = "Internal Server Error";
	}
	Log::get(logDEBUG) << __FUNCTION__  << " Generating response for " << req_.getReqTarget() << std::endl;
}

Response::~Response() { }

void Response::writeBaseHeaders_()
{
	append_("Server: " + std::string(WEBSERV_ID) + "\r\n");
	append_("Date: " + getStrDate() + "\r\n");
	append_("Connection: close\r\n");
}

void Response::writeContentType_(std::string value)
{
	append_("Content-Type: " + value + "\r\n");
}

void Response::writeContentLength_(long value)
{
	append_("Content-Length: " + ft_itoa(value) + "\r\n");
}

void Response::writeAllow_()
{
	append_("Allow: ");
	for (size_t i = 0; i < req_.getLocation()->methods.size(); i++)
    {
	    append_(req_.getLocation()->methods[i]);
	    if (i < req_.getLocation()->methods.size() - 1)
    	    append_(", ");
    }
	append_("\r\n");
}

void Response::writeStatusLine_(int statusCode)
{
	append_("HTTP/1.1 " + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "\r\n");
}

void Response::writeThisHeader_(std::string name, std::string value)
{
	append_(name + ": " + value + "\r\n");
}

void Response::writeHeadersEnd_()
{
	append_("\r\n");
	headersBuilt_ = true;
}

int Response::writeErrorPage(int statusCode)
{
	statusCode_ = statusCode;
	if (headersBuilt_)
		return 0;
	nbytes_ = 0;
	std::string body = "<html>"
					"<head><title>" + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</title></head>"
					"<body bgcolor=\"white\">"
					"<center><h1>"+ ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</h1></center>"
					"<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
				    "</body>"
		 			"</html>";
	appendHeaders(statusCode, "text/html", body.size());
	append_(body);
	return (nbytes_);
}

void Response::writeErrorBody(int statusCode)
{
	std::string body = "<html>"
					"<head><title>" + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</title></head>"
					"<body bgcolor=\"white\">"
					"<center><h1>"+ ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</h1></center>"
					"<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
				    "</body>"
		 			"</html>";
	writeContentLength_(body.size());
	writeHeadersEnd_();
	append_(body);
}

void Response::append_(std::string str)
{
	if (str.size() + nbytes_ > bufSize_)
		return ;
	ft_strncpy(buf_ + nbytes_, str.c_str(), str.size());
	nbytes_ += str.size();
}

void Response::append_(const char str[], unsigned int size)
{
	if (size + nbytes_ > bufSize_)
		return ;
	ft_strncpy(buf_ + nbytes_, str, size);
	nbytes_ += size;
}

void Response::appendHeaders(int statusCode, std::string contentType, unsigned int contentLength)
{
	writeStatusLine_(statusCode);
	writeBaseHeaders_();
	writeContentType_(contentType);
	writeContentLength_(contentLength);
	writeHeadersEnd_();
}

int Response::getStatusCode() const {
	return statusCode_;
}
