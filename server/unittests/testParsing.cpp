#include "unittests.hpp"

static void assertThrow(std::string path, std::string testName)
{
	Parsing::getInstance()->setFile(path);
	try {
		Parsing::getInstance()->parseConfig();
	} catch (Parsing::ParsingException &e) {
		std::cout << "\033[1;32mSuccess\033[0m > " << testName << " > exception as expected" << std::endl;
		return ;
	}
	std::cout << "\033[1;31mFail\033[0m: > " << testName << " > expected a throw." << std::endl;
	delete Parsing::getInstance();
}

void testParsing()
{
	std::cout << std::endl << "\033[1;35m" <<  __FUNCTION__  << "\033[0m" << std::endl;

	assertThrow("./parsing/idoexistright", "file does not exist");
	assertThrow("./parsing/nope.nope", "extension doesnt match .conf");
	assertThrow("./parsing/semicolonMissing.conf", "missing semicolon");
	assertThrow("./parsing/duplicateLocation.conf", "adding the same location twice");
	assertThrow("./parsing/duplicateRoot.conf", "adding the same root twice");
	assertThrow("./parsing/invalidPort.conf", "[emerg] invalid port");
	assertThrow("./parsing/doubleSemicolon.conf", " duplicate semicolon");
	assertThrow("./parsing/noServer.conf", " no server provide");
	assertThrow("./parsing/noArguments.conf", " no arguments provide");
	assertThrow("./parsing/missingBrackets.conf", " missing brackets");
	assertThrow("./parsing/missingBrackets.conf", " missing brackets");
}
