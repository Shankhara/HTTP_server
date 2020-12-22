#ifndef WEBSERV_IFD_HPP
#define WEBSERV_IFD_HPP
#include <vector>
#include "../Logger.hpp"
#include "../Utils.hpp"

class FileDescriptor {
protected:
	int fd_;
	unsigned long lastEventTimer_;

public:
	virtual void	onEvent() = 0;
	virtual ~FileDescriptor(){};
	int getFd() const {
		return fd_;
	}
	void setFd(int fd) {
		fd_ = fd;
	}
	unsigned long getLastEventTimer() const {
		return (lastEventTimer_);
	}
	void setLastEventTimer() {
		lastEventTimer_ = getTime();
	}
};

#endif
