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
#include "RespException.hpp"

class RespGet: public RespFile
{
protected:
	std::string autoIndexBody_;
	const Parsing::location *location_;
	std::string reqTarget_;

	void 		reachResource_();
	int 		writeAutoIndex_(stds);
	void 		writeHeaders_();

	std::string doAutoIndexTemplate_(stds path);
	std::string	returnLineTemplate_(stds body, std::list<stds>::iterator first, struct stat t_stat);

public:
	RespGet(const Request &r, char[], unsigned int);
	~RespGet();
	virtual int readResponse();
	virtual void build();
};
