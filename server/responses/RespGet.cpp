#include "RespGet.hpp"

RespGet::RespGet(const Request &r): Response(r) {
}

RespGet::~RespGet() {}

void RespGet::build() {
	Parsing::location *location = req_.getLocation();
	std::string path = location->root + req_.getReqTarget();
	Log().Get(logDEBUG) << __FUNCTION__  << " PATH: " << path;

	int fd = open(path.c_str(), O_RDONLY);
	if (fd == -1)
	{
		Log().Get(logINFO) << __FUNCTION__  << " unable to open: " << strerror(errno);
		error404();
		return ;
	}

	struct stat st;
	fstat(fd, &st);
	setHeaderContentLength(st.st_size);
	Log().Get(logDEBUG) << __FUNCTION__ << "Filesize: " << st.st_size;
	setHeaderContentType(std::string("text/html")); //TODO
	putHeaders();

	char buf[256];
	int nbytes;
	while ((nbytes = read(fd, buf, 255)) > 0)
		msg_.append(buf, nbytes);
	if (nbytes < 0)
		Log().Get(logERROR) << __FUNCTION__  << " read error " << strerror(errno);
	close(fd);
}
