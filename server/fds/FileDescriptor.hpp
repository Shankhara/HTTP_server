#ifndef WEBSERV_IFD_HPP
#define WEBSERV_IFD_HPP
#include <vector>
#include "../Logger.hpp"

class FileDescriptor {
protected:
	int fd_;
public:
	virtual void	onEvent() = 0;
	virtual ~FileDescriptor(){
		Log().Get(logDEBUG) << "Filedescriptor deleted " << fd_;
	};
	int getFd() const {
		return fd_;
	}
	void setFd(int fd) {
		fd_ = fd;
	}
};

#endif
