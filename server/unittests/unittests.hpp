#ifndef WEBSERV_UNITTESTS_HPP
#define WEBSERV_UNITTESTS_HPP
#include <vector>
#include "tests.hpp"
#include "../parsing/Parsing.hpp"
#include "../CGIExec.hpp"
//#include "../Request.hpp"
#include "../Response.hpp"
#include "../GetMethod.hpp"
#include "../fds/Client.hpp"
#include "../fds/Listener.hpp"

void testRequest();
void testResponse();
void testCGI();
void testParsing();
std::vector<Parsing::server> *createVirtualHosts();

#endif

