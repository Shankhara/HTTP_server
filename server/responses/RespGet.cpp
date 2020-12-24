#include "RespGet.hpp"

RespGet::RespGet(const Request &r, char buf[], unsigned int bufSize): Response(r, buf, bufSize) {
	fd_ = 0;
}

RespGet::~RespGet() {
	if (fd_ > 0)
		close(fd_);
}

int RespGet::readResponse() {
	if (fd_ == -1)
		return 0;
	if (headersBuilt_ == false)
	{
		Parsing::location *location = req_.getLocation();
		if (location == 0)
		{
			Log().Get(logERROR) << __FUNCTION__  << "Location is null";
			return (0);
		}
		std::string path = location->root + req_.getReqTarget();
		Log().Get(logDEBUG) << __FUNCTION__  << " PATH: " << path;

		fd_ = open(path.c_str(), O_RDONLY);
		if (fd_ == -1)
		{
			Log().Get(logINFO) << __FUNCTION__  << " unable to open: " << strerror(errno);
			return (error404());
		}

		struct stat st;
		fstat(fd_, &st);
		setHeaderContentLength(st.st_size);
		Log().Get(logDEBUG) << __FUNCTION__ << "Filesize: " << st.st_size;
		setHeaderContentType(std::string("text/html")); //TODO
		putHeaders();
		headersBuilt_ = true;
	}
	int nbytes = read(fd_, buf_ + nbytes_, bufSize_ - (nbytes_ + 1));
	if (nbytes < 0)
		Log().Get(logERROR) << __FUNCTION__  << " read error " << strerror(errno);
	return (nbytes);
}
