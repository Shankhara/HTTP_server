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
	std::string body = doAutoIndexTemplate_();
	appendHeaders("text/html", body.size());
	append_(body); //TODO: tterrail if body.size > BUFFER_SIZE content-Length will not match body sent;
	return (nbytes_);
}