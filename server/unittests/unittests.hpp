#ifndef WEBSERV_UNITTESTS_HPP
#define WEBSERV_UNITTESTS_HPP
#include <vector>
#include "tests.hpp"
#include "../parsing/Parsing.hpp"
#include "../CGIExec.hpp"
//#include "../Request.hpp"
#include "../fds/Client.hpp"
#include "../fds/Listener.hpp"
#include "../fds/Mime.hpp"
#include "../responses/Response.hpp"
#include "../responses/RespGet.hpp"
#include "../responses/RespPut.hpp"
#include "../responses/RespPost.hpp"
#include "../responses/RespDelete.hpp"
#include "../responses/RespTrace.hpp"
#include "../responses/RespOptions.hpp"

void testRequest();
void testResponse();
void testCGI();
void testParsing();
std::vector<Parsing::server> *createVirtualHosts();

#endif
