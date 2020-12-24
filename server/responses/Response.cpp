#include "Response.hpp"

Response::Response(const Request & r, char buf[], unsigned int bufSize) : req_(r), buf_(buf), bufSize_(bufSize)
{
	headersBuilt_ = false;
	statusCode_ = req_.getStatusCode();
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

	setBaseHeaders();
}

Response::~Response() { }

void Response::setBaseHeaders()
{
	headersToPut_.push_back("Server: " + std::string(WEBSERV_ID));
	headersToPut_.push_back("Date: " + getStrDate());
}

void Response::setHeaderContentType(std::string value) {
	headersToPut_.push_back("Content-Type: " + value);
}

void Response::setHeaderContentLength(long value) {
	headersToPut_.push_back("Content-Length: " + ft_itoa(value));
}

void Response::putHeaders()
{
	append_("\r\n");
	for (size_t i = 0; i < headersToPut_.size(); i++)
	{
		append_(headersToPut_[i]);
		append_("\r\n");
	}
	append_("\r\n");
}

void Response::appendStatusCode()
{
	append_(req_.getVersion());
	append_(" ");
	append_(ft_itoa(statusCode_));
	append_(" ");
	append_(statusMap_[statusCode_]);
}


int Response::error404() {
	nbytes_ = 0;
	statusCode_ = 404;
	appendStatusCode();
	std::string body = "<html>"
					   "<head><title>404 Not Found</title></head>"
					   "<body bgcolor=\"white\">"
					   "<center><h1>404 Not Found</h1></center>"
						"<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
					   "</body>"
					   "</html>";
	setHeaderContentLength(body.size());
	setHeaderContentType("text/html");
	putHeaders();
	append_(body);
	return (nbytes_);
}

void Response::append_(std::string str) {
	//TODO: check size ofc
	strncpy(buf_ + nbytes_, str.c_str(), str.size());
	nbytes_ += str.size();
}

void Response::append_(char str[], int size) {
	//TODO: check size ofc
	strncpy(buf_ + nbytes_, str, size);
	nbytes_ += size;
}

