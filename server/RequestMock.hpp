#ifndef WEBSERV_REQUESTMOCK_HPP
#define WEBSERV_REQUESTMOCK_HPP
#include "fds/Client.hpp"

class RequestMock {
private:
	Client &client_;
	std::string contentLength_;
	std::string requestMethod_;
public:
	const std::string &getRequestMethod() const;

	void setRequestMethod(const std::string &requestMethod);

public:
	void setContentLength(const std::string &contentLength);

public:
	Client &getClient() const;

public:
	virtual ~RequestMock();
	RequestMock(Client &);
	std::string getHeaderContentLength() const;

};


#endif //WEBSERV_REQUESTMOCK_HPP
