#include "server.hpp"

server::server(std::string serverName, const char* port = 0) : name_(serverName), port_(port), sockfd_(-1) { }

server::~server() { }

void server::listen_()
{
	int status;
    struct addrinfo	hints_;

	memset(&hints_, 0, sizeof(hints_));
	hints_.ai_family = AF_UNSPEC;
	hints_.ai_socktype = SOCK_STREAM;
	hints_.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port_, &hints_, &res_)))
	{
		std::cerr << "server:start -> error in getaddrinfo(), error code: " << status << std::endl;
	    exit(8);
	}

	if ((sockfd_ = socket(res_->ai_family, res_->ai_socktype, res_->ai_protocol)) == -1)
	{
		std::cerr << "server:start -> error in socket()\n";
	    exit(8);
    }


	if ((bind(sockfd_, res_->ai_addr, res_->ai_addrlen)) == -1)
	{
		std::cerr << "server:start -> error in bind() " << strerror(errno) << std::endl;
	    exit(8);
    }

    if (listen(sockfd_, 20) == -1)
	{
	    std::cerr << "server:start -> error in listen()\n";
	    exit(8);
	}
}

void server::run_()
{
	fd_set					master;
	fd_set					conn_fds;
	struct sockaddr_storage remoteaddr;
	socklen_t 				addrlen;
	int						newfd;
	int 					fdmax = sockfd_;
	int 					nbytes = 0;
	char					buf[256];
	FD_ZERO(&master);
	FD_ZERO(&conn_fds);
	FD_SET(sockfd_, &master);

	while (1)
	{
		conn_fds = master;
		if (select(fdmax+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}
		std::cerr << "server::run -> select UNLOCK " << std::endl;
		for (int i = 0; i <= fdmax; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				if (i == sockfd_)
				{
					addrlen = sizeof(remoteaddr);
					if ((newfd = accept(sockfd_, (struct sockaddr *)(&remoteaddr), &addrlen)) == -1)
					{
						perror("in accept");
					    exit(8);
					}

					if (fcntl(sockfd_, F_SETFL, O_NONBLOCK) == -1)
					{
					    perror("server:start -> error in fcntl()");
					    exit(8);
				    }

					int yes = 1;
					if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
					{
					    perror("in setsockopt()");
					    exit(8);
					} 

					FD_SET(newfd, &master);
					if (newfd > fdmax) 
						fdmax = newfd;
					std::cerr << "server::run -> new conn " << std::endl;
				}
				else
				{
					nbytes = recv(i, buf, sizeof(buf), 0);
					std::cerr << "server::run -> RECV " << nbytes << std::endl;
					if (nbytes <= 0)
					{
						if (nbytes == 0)
							std::cerr << "server::run -> closed" << std::endl;
						else
							perror("client recv");
						close(i);
						FD_CLR(i, &master);
					}
					else
					{
						if (FD_ISSET(i, &conn_fds))
						{
							if (send(i, buf, nbytes, 0) == -1)
								std::cerr << "server::run -> response sent error: " << strerror(errno) << std::endl;
							else
								std::cerr << "server::run -> response SENT" << std::endl;
						}
						else
							std::cerr << "server::run -> FD NOT READY?" << std::endl;
					}
				}
			}
		}
	}
}


void server::start()
{
	server::listen_();
	server::run_();
}
