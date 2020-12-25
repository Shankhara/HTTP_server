#include "RespGet.hpp"

std::string RespGet::doAutoIndexTemplate_() {
	std::string body = "<html>"
					   "<head>"
					   "<title>WebServ AutoIndex</title>"
					   "</head>"
					   "<body><h3>it works!</h3>"
					   "</html>";
	return body;
}

int RespGet::writeAutoIndex_() {
	nbytes_ = 0;
	appendStatusCode(200);
	appendBaseHeaders();
	setHeaderContentType("text/html");
	std::string body = doAutoIndexTemplate_();
	setHeaderContentLength(body.size());
	appendHeadersEnd();
	append_(body); //TODO: tterrail if body.size > BUFFER_SIZE content-Length will not match body sent;
	return (nbytes_);
}