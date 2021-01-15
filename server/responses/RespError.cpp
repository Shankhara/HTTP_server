#include "RespError.hpp"

RespError::RespError(int statusCode, const Request &r, char *buf, unsigned int bufSize): RespFile(r, buf, bufSize)
{
	statusCode_ = statusCode;
	setFilePath_();
	build();
}

RespError::~RespError() {}

int RespError::readResponse()
{
	nbytes_ = 0;
	
	if (fd_ == 0)
	{
		if (!headersBuilt_) {
			writeDefaultErrorPage_(statusCode_);
			return nbytes_;
		}
		return 0;
	}
	if (!headersBuilt_)
		writeHeaders_(payloadSize_);
	if (req_.getMethod() != "HEAD")
		return read_();
	return nbytes_;
}

void RespError::writeDefaultErrorPage_(int statusCode)
{
	static const std::string body = 		"<html>"
				  "<head><title>" + ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</title></head>"
																						 "<body bgcolor=\"white\">"
																						 "<center><h1>"+ ft_itoa(statusCode) + " " + statusMap_[statusCode] + "</h1></center>"
																																							  "<hr><center>"+ std::string(WEBSERV_ID) +"</center>"
																																																	   "</body>"
																																																	   "</html>";
	writeHeaders_(body.size());
	if (req_.getMethod() == "HEAD")
		return ;
	append_(body);
}

void RespError::writeStatusRelatedHeaders_() {

	if (statusCode_ == 401)
		writeThisHeader_("WWW-Authenticate", "Basic realm=\"simple\"");
	if (statusCode_ == 405)
		writeAllow_();
	if (statusCode_ == 406)
		writeThisHeader_("Accept-charset", "utf-8");
	if (statusCode_ == 503)
		writeThisHeader_("Retry-After", "10");
}

void RespError::setFilePath_() {
	const Parsing::server *server = req_.getServer();
	if (server == 0)
		return ;
	for (size_t i = 0; i < server->error_pages.size(); i++)
		if (server->error_pages[i].first == statusCode_)
			filePath_ = server->error_pages[i].second;
}

void RespError::build() {
	if (filePath_.empty())
		return ;

	struct stat st;
	try {
		openFile_(O_RDONLY, 500);
		fstat(fd_, &st);
		payloadSize_ = st.st_size;
	}catch (const RespException &) {
		filePath_ = "";
		fd_ = 0;
	}
}

void RespError::writeHeaders_(size_t s)
{
	initHeaders();
	writeThisHeader_("Content-Type", "text/html");
	writeStatusRelatedHeaders_();
	writeContentLength_(s);
	writeHeadersEnd_();
}

