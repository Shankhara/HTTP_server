#include "Log.hpp"

Log* Log::instance_ = 0;

static std::string timeStamp() {
	time_t		stamp;
	struct tm		*timeinfo;
	char			timestamp[20];

	time(&stamp);
	timeinfo = localtime(&stamp);
	strftime(timestamp, 20, "%FT%TZ", timeinfo);
	return timestamp;
}

static std::string printCurrentLevel(TLogLevel level) {
	switch (level) {
		case logFATAL: return "FATAL";
		case logERROR: return "ERROR";
		case logWARNING: return "WARN";
		case logINFO: return "INFO";
		case logDEBUG: return "DEBUG";
	}
	return "NONE";
}

Log::Log() {
	logLevel_ = logINFO;
}

Log::~Log() {}

std::ostream &Log::get(TLogLevel level) {

	Log *logger = Log::getInstance();
	if (level < logger->getLogLevel())
	{
		std::cerr.setstate(std::ios_base::badbit);
		return std::cerr;
	}
	else
		std::cerr.clear();
	std::cerr << "[" << timeStamp();
	std::cerr << "] " << printCurrentLevel(level) << ": ";
	return std::cerr;
}

Log *Log::getInstance() {
	if (instance_ == 0)
		instance_ = new Log();
	return (instance_);
}

void Log::setLevel(TLogLevel level) {
	Log *logger = Log::getInstance();
	logger->logLevel_ = level;
}

TLogLevel Log::getLogLevel() const {
	return logLevel_;
}


