#include "server.hpp"

server::server(std::string serverName, const char* port = NULL) : _name(serverName), _port(port), _sockfd(-1) 
{
	memset(&hints, 0, sizeof hints);
}

server::~server() { }

int server::start()
{
	int status;
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	//Pour set une IP, enlever AI_PASSIVE et remplacer NULL
	if ((status = getaddrinfo(NULL, _port, &hints, &_res)))
	{
		std::cerr << "server:start -> error in getaddrinfo(), error code: " << status << std::endl;
	    exit(8);
	}

	if ((_sockfd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol)) == -1)
	{
		std::cerr << "server:start -> error in socket()\n";
	    exit(8);
    }
	
	if ((bind(_sockfd, _res->ai_addr, _res->ai_addrlen)) == -1)
	{
		std::cerr << "server:start -> error in bind()\n";
	    exit(8);
    }

	int yes = 1;
	// Pour contrer le "Add_ress already in use" error message
	if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
	{
	    std::cerr << "server:start -> error in setsockopt()\n";
	    exit(8);
	} 

    if (listen(_sockfd, 20) == -1)
	{
	    std::cerr << "server:start -> error in listen()\n";
	    exit(8);
	} 
	
	//Waiting for select() ! Thibaud a toi de jouer :D

    return (0);
}
