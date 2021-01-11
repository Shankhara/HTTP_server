#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include <string.h>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <errno.h>
#include <vector>
#include <arpa/inet.h>
#include "Log.hpp"
#include "fds/FileDescriptor.hpp"

#define READ_TIMEOUT 10

class Server
{
private:
	static Server					*instance_;
    std::vector<FileDescriptor*>	fds_;
	int 							fdmax_;
	fd_set							master_;
    void 							run_();
	void 							garbageCollector_();
	Server();

public:
	~Server();
	static Server		*getInstance();
    void 				start();
    void 				addFileDescriptor(FileDescriptor *);
    void 				stop();
	void 				deleteFileDescriptor(int);
};

#endif
