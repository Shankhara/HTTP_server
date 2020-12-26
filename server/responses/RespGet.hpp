#ifndef WEBSERV_GETMETHOD_HPP
#define WEBSERV_GETMETHOD_HPP
#include "Response.hpp"
#include "../Logger.hpp"
#include "../Utils.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

class RespGet: public Response {
private:
	int			fd_;
	void 		openFile_(Parsing::location *location);
	int		 	readFile_();
	int 		writeAutoIndex_(stds path);
	std::string doAutoIndexTemplate_(stds path);
	std::string	returnLineTemplate_(stds body, std::list<stds>::iterator first, struct stat t_stat);
public:
	RespGet(const Request &r, char[], unsigned int);
	~RespGet();
	int readResponse();

};


#endif //WEBSERV_GETMETHOD_HPP
