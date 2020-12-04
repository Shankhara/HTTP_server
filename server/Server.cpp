#include "Server.hpp"

Server::~Server()
{
	//close(master_);
}

Server::Server(std::string serverName, const char* port = 0):
		name_(serverName),
		port_(port), sockfd_(-1) {
	clients_.reserve(FD_SETSIZE);
	for (int i = 0; i < FD_SETSIZE; i++)
	{
		clients_.push_back(Client(i));
	}
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
		Log().Get(logERROR) << "server::listen -> error in getaddrinfo(), error code: " << strerror(errno);
	    exit(8);
	}

	if ((sockfd_ = socket(res_->ai_family, res_->ai_socktype, res_->ai_protocol)) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in socket()\n";
	    exit(8);
    }

	if ((bind(sockfd_, res_->ai_addr, res_->ai_addrlen)) == -1)
	{
		Log().Get(logERROR) << "server:start -> error in bind() " << strerror(errno);
	    exit(8);
    }

    if (listen(sockfd_, FD_SETSIZE) == -1)
	{
    	Log().Get(logERROR) << "server:start -> error in listen() " << strerror(errno);
	    exit(8);
	}
}

void Server::run_()
{
	fd_set					conn_fds;

	FD_ZERO(&master_);
	FD_ZERO(&conn_fds);
	FD_SET(sockfd_, &master_);

	fdmax_ = sockfd_;
	while (1)
	{
		conn_fds = master_;
		if (select(fdmax_+1, &conn_fds, NULL, NULL, NULL) == -1)
		{
			Log().Get(logERROR) << "server::run -> select " << strerror(errno) << " maxfd: " << fdmax_;
			exit(4);
		}
		Log().Get(logDEBUG) << "server::run -> select UNLOCK ";
		for (int i = 0; i <= fdmax_; i++)
		{
			if (FD_ISSET(i, &conn_fds))
			{
				if (i == sockfd_)
					onClientConnect();
				else
				{
					if (clients_[i].onDataReceived() <= 0)
						onClientDisconnect(i);
				}
			}
		}
	}
}

void Server::start()
{
	Log().Get(logINFO) << "Server " << name_ << " started on port " << port_ << " (maxconn: " << FD_SETSIZE << ")";
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
		Log().Get(logERROR) << "server::onClientConnect " << strerror(errno);
		exit(8);
	}

	FD_SET(newfd, &master_);
	if (newfd > fdmax_)
		fdmax_ = newfd;
	clients_[newfd].setAddr(remoteaddr);
}

void Server::onClientDisconnect(int fd_) {
	Log().Get(logDEBUG) << "ClientDisconnect: " << fd_;
	close(fd_);
	FD_CLR(fd_, &master_);
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
