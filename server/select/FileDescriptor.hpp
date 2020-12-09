#ifndef WEBSERV_IFD_HPP
#define WEBSERV_IFD_HPP
#include <vector>

class FileDescriptor {
protected:
	int fd_;
public:
	virtual void	onEvent() = 0;

	int getFd() const {
		return fd_;
	}
	void setFd(int fd) {
		fd_ = fd;
	}
};

#endif
