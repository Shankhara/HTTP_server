#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP
#include <ostream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <cstdlib>


enum TLogLevel {
	logDEBUG, logINFO, logWARNING, logERROR, logFATAL
};

class Log
{
private:
	static TLogLevel level_;
	std::ostringstream os;
	Log(const Log&);
	Log& operator =(const Log&);
	TLogLevel messageLevel_;

public:
	Log();
	virtual ~Log();
	std::ostringstream& Get(TLogLevel level = logINFO);
	static void setLevel(TLogLevel level);
};




#endif //WEBSERV_LOGGER_HPP
