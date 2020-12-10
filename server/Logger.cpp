#include "Logger.hpp"

TLogLevel Log::level_ = logINFO;

static std::string timeStamp() {
	std::time_t		stamp;
	struct tm		*timeinfo;
	char			timestamp[20];

	time(&stamp);
	timeinfo = localtime(&stamp);
	strftime(timestamp, 20, "%FT%TZ", timeinfo);
	return timestamp;
}

static std::string printCurrentLevel(TLogLevel level) {
	switch (level) {
		case logERROR: return "ERROR";
		case logWARNING: return "WARN";
		case logINFO: return "INFO";
		case logDEBUG: return "DEBUG";
	}
	return "NONE";
}

std::ostringstream& Log::Get(TLogLevel level)
{
	messageLevel_ = level;
	os << "[" << timeStamp();
	os << "] " << printCurrentLevel(level) << ": ";
	return os;
}
Log::~Log()
{
	os << std::endl;
	if (messageLevel_ >= level_)
		std::cout << os.str().c_str();
}

Log::Log() {
	messageLevel_ = logDEBUG;
}

Log::Log(const Log &) {}

Log &Log::operator=(const Log &) {
	return *this;
}

void Log::setLevel(TLogLevel level) {
	level_ = level;
}

/*void Log::SetLevel(TLogLevel level) {
	//TODO
}*/
