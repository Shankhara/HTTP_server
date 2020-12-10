#include "RequestMock.hpp"

RequestMock::RequestMock(Client &client): client_(client) {}

RequestMock::~RequestMock() {

}

Client &RequestMock::getClient() const {
	return client_;
}

std::string RequestMock::getHeaderContentLength() const {
	return contentLength_;
}

void RequestMock::setContentLength(const std::string &contentLength) {
	contentLength_ = contentLength;
}

const std::string &RequestMock::getRequestMethod() const {
	return requestMethod_;
}

void RequestMock::setRequestMethod(const std::string &requestMethod) {
	requestMethod_ = requestMethod;
}

