#pragma once

#include "Request.hpp"

#define OK_200 200
#define CREATED_201 201
#define ACCEPTED_202 202
#define NO_CONTENT_204	204
#define USE_PROXY_305 305	
#define BAD_REQUEST_400 400
#define UNAUTHORIZED_401 401
#define FORBIDDEN_403 403
#define NOT_FOUND_404 404
#define METHOD_NOT_ALLOWED_405 405
#define NOT_ACCEPTABLE_406 406
#define CONFLICT_409 409
#define REQUEST_ENTITY_TOO_LARGE_413 413
#define REQUEST_URI_TOO_LONG_414 414
#define SSL_CERTIFICATE_ERROR_495 495
#define INTERNAL_ERROR_500 500
#define NOT_IMPLEMENTED_501 501
#define SERVICE_UNAVAILABLE_503 503
#define HTTP_VERSION_NOT_SUPPORTED_505 505

class Response
{
	private:
	Request request_;
	std::string msg_;

	public:
	Response(const Request &);
	~Response();

	void createResponse();
	void statusLine();

};
