#include "Server.hpp"
#include "Logger.hpp"

int main() {
	//Log::SetLevel(logDEBUG);
	Server local("local", "8080");
	local.start();
}

