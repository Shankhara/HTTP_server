#include "Response.hpp"

Response::Response(const Request & r, char buf[], unsigned int bufSize) : req_(r), buf_(buf), bufSize_(bufSize)
{
	nbytes_= 0;
	headersBuilt_ = false;
	statusMap_[200] = "OK";
	statusMap_[201] = "Created";
	statusMap_[202] = "Accepted";
	statusMap_[204] = "No Content";
	statusMap_[400] = "Bad Request";
	statusMap_[401] = "Unauthorized";
	statusMap_[404] = "Not Found";
	statusMap_[405] = "Method Not Allowed";
	statusMap_[413] = "Request Entity Too Large";
	statusMap_[414] = "Request-URI Too Long";
	statusMap_[500] = "Internal Server Error";
}

Response::~Response() { }

void Response::appendBaseHeaders()
{
	append_("Server: " + std::string(WEBSERV_ID) + "\r\n");
	append_("Date: " + getStrDate() + "\r\n");
}

void Response::setHeaderContentType(std::string value) {
	append_("Content-Type: " + value + "\r\n");
}

void Response::setHeaderContentLength(long value) {
	append_("Content-Length: " + ft_itoa(value) + "\r\n");
}

void Response::appendStatusCode(int statusCode)
{
	append_(req_.getVersion() + " " + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "\r\n");
}

void Response::appendHeadersEnd()
{
	append_("\r\n");
}

int Response::writeErrorPage(int statusCode) {
	nbytes_ = 0;
	appendStatusCode(statusCode);
	appendBaseHeaders();
	std::string body = "<html>"
					"<head><title>" + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</title></head>"
					"<body bgcolor=\"white\">"
					"<center><h1>"+ ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</h1></center>"
					"<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
				    "</body>"
		 			"</html>";
	setHeaderContentLength(body.size());
	setHeaderContentType("text/html");
	appendHeadersEnd();
	append_(body);
	return (nbytes_);
}

void Response::append_(std::string str) {
	if (str.size() + nbytes_ > bufSize_)
		return ;
	strncpy(buf_ + nbytes_, str.c_str(), str.size());
	nbytes_ += str.size();
}

void Response::append_(char str[], unsigned int size) {
	if (size + nbytes_ > bufSize_)
		return ;
	strncpy(buf_ + nbytes_, str, size);
	nbytes_ += size;
}

