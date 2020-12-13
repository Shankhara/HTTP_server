#include "unittests.hpp"

int main()
{
	testParsingRequestSequence();
	testParsingRequestLine();
	testParsingRequestReceivedAtOnce();
	return 0;
}
