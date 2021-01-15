#include "RespException.hpp"

RespException::RespException(int status): statusCode_(status) {}

RespException::RespException(int status, std::string loc): statusCode_(status), location_(loc) {}

RespException::~RespException() throw() {}

int RespException::getStatusCode() const {
	return statusCode_;
}

const char *RespException::what() const throw() {
	return "error";
}

const std::string &RespException::getLocation() const {
	return location_;
}
