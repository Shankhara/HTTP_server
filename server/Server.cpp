#include "Server.hpp"

Server::~Server()
{
	//close(master_);
}

Server::Server(std::string serverName, const char* port = 0):
		name_(serverName),
		port_(port), sockfd_(-1) {
	clients_ = std::vector<Client>();
	clients_.reserve(FD_SETSIZE);
}


void Server::listen_()
{
	int status;
    struct addrinfo	hints;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((status = getaddrinfo(NULL, port_, &hints, &res_)))
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

    if (listen(sockfd_, FD_SETSIZE) == -1)
	{
	    std::cerr << "server:start -> error in listen()\n";
	    exit(8);
	}
}

void Server::run_()
{
	fd_set					conn_fds;
	int 					nbytes = 0;
	char					buf[256];

	FD_ZERO(&master_);
	FD_ZERO(&conn_fds);
	FD_SET(sockfd_, &master_);

	fdmax_ = sockfd_;
	while (1)
	{
		conn_fds = master_;
		if (select(fdmax_+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			std::cerr << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_ << std::endl;
			exit(4);
		}
		std::cerr << "server::run -> select UNLOCK " << std::endl;
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				if (i == sockfd_)
					onClientConnect();
				else
				{
					nbytes = recv(i, buf, sizeof(buf), 0);
					std::cerr << "server::run -> RECV " << nbytes << std::endl;
					if (nbytes <= 0)
					{
						if (nbytes < 0)
							perror("client recv");
						onClientDisconnect(i);
					}
					else
					{
						if (FD_ISSET(i, &conn_fds))
							clients_[i].onDataReceived(buf, nbytes);
						else
							std::cerr << "server::run -> FD NOT READY?" << std::endl;
					}
				}
			}
		}
	}
}


void Server::start()
{
	std::cout << "Server: " << name_ << " started on port " << port_ << " (maxconn: " << FD_SETSIZE << ")" << std::endl;
	Server::listen_();
	Server::run_();
}

void Server::onClientConnect() {
	socklen_t 				addrlen;
	struct sockaddr_storage remoteaddr;
	int						newfd;

	addrlen = sizeof(remoteaddr);

	if ((newfd = accept(sockfd_, (struct sockaddr *)(&remoteaddr), &addrlen)) == -1)
	{
		perror("in accept");
		exit(8);
	}

	FD_SET(newfd, &master_);
	if (newfd > fdmax_)
		fdmax_ = newfd;
	clients_[newfd] = Client(newfd);
}

void Server::onClientDisconnect(int fd_) {
	std::cerr << "ClientDisconnect: " << fd_ << std::endl;
	close(fd_);
	FD_CLR(fd_, &master_);
	clients_[fd_] = Client();
}


/* TODO: Test if we need to either reuseaddr for client sockets since our FD_SETSIZE is way smaller than 65k - 1024
 * or if recv is blocking on 0 read like Mac OS X, so far everything was ok
 * if (fcntl(sockfd_, F_SETFL, O_NONBLOCK) == -1)
{
	perror("server:start -> error in fcntl()");
	exit(8);
}

int yes = 1;
if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
{
	perror("in setsockopt()");
	exit(8);
}*/
