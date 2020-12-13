#include "../tests.hpp"
#include "CGI.hpp"

int main(int argc, char *argv[]){
	(void)argv;
	if (argc == 2)
		Log().setLevel(logDEBUG);
	testCGI();
}
