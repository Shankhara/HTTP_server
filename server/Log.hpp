#ifndef WEBSERV_LOG_HPP
#define WEBSERV_LOG_HPP
#include <iostream>

enum TLogLevel {
	logDEBUG, logINFO, logWARNING, logERROR, logFATAL
};

class Log {

private:
	static Log *instance_;
	TLogLevel logLevel_;
	TLogLevel getLogLevel() const;
	Log();

public:
	~Log();
	static std::ostream 		&get(TLogLevel);
	static Log 					*getInstance();
	void 						setLevel(TLogLevel);
};

std::ostream		&operator<<(std::ostream &out, Log *s);

#endif //WEBSERV_LOG_HPP
