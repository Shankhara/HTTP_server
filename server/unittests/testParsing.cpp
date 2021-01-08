#include "unittests.hpp"

static void assertThrow(std::string path, std::string testName)
{
	Parsing p(path);
	try {
		p.parseConfig();
	} catch (Parsing::ParsingException &e) {
		std::cout << "\033[1;32mSuccess\033[0m > " << testName << " > exception as expected" << std::endl;
		return ;
	}
	std::cout << "\033[1;31mFail\033[0m: > " << testName << " > expected a throw." << std::endl;
}

static void assertLoad(Parsing &p, std::string testName)
{
	try {
		p.parseConfig();
	} catch (Parsing::ParsingException &e) {
		std::cout << "\033[1;31mFail\033[0m: > " << testName << "> expecting no exception got " << e.what() << std::endl;
	}
}

void testDuplicateServerName()
{
	std::string testName = __FUNCTION__ ;
	Parsing p("./parsing/duplicateServerName.conf");
	assertLoad(p, testName);
	assertEqual(p.getServers().size(), (unsigned long)2, testName);
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


//	testDuplicateServerName();
}
