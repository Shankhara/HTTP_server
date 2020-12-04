#ifndef WEBSERV_LOGGER_HPP
#define WEBSERV_LOGGER_HPP
#include <ostream>
#include <ctime>
#include <iostream>
#include <sstream>


enum TLogLevel {
	logERROR, logWARNING, logINFO, logDEBUG
};

class Log
{
public:
	Log();
	virtual ~Log();
	std::ostringstream& Get(TLogLevel level = logINFO);
	//static void SetLevel(TLogLevel level);
private:
	std::ostringstream os;
	Log(const Log&);
	Log& operator =(const Log&);
	TLogLevel messageLevel_;
};




#endif //WEBSERV_LOGGER_HPP
