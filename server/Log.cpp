#include "Log.hpp"

Log* Log::instance_ = 0;

static std::string timeStamp() {
	struct timeval	tv;
	struct tm		timeinfo;
	char			timestamp[20];

	gettimeofday(&tv, NULL);
	strptime(ft_itoa(static_cast<unsigned long>(tv.tv_sec)).c_str(), "%s", &timeinfo);
	strftime(timestamp, 20, "%FT%TZ", &timeinfo);
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
	std::cerr << "[" << timeStamp() << "] " << printCurrentLevel(level) << ": ";
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


