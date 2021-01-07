#pragma once

#include "RespFile.hpp"
#include "../Log.hpp"
#include "../Utils.hpp"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

class RespGet: public RespFile
{
	private:
	int fd_;
	size_t payLoadSize_;
	std::string autoIndexBody_;
	const Parsing::location *location_;
	std::string reqTarget_;

	void 		openFile_();
	int		 	readFile_();
	int 		writeAutoIndex_(stds);
	void 		makeResponse_();

	std::string doAutoIndexTemplate_(stds path);
	std::string	returnLineTemplate_(stds body, std::list<stds>::iterator first, struct stat t_stat);

	public:
	RespGet(const Request &r, char[], unsigned int);
	~RespGet();
	virtual int readResponse();
};
