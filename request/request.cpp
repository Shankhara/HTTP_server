#include "request.hpp"

int parseRequestLine(std::string & line)
{
	line.erase(line.find('\r'));
    requestLine_ = explode(line, ' ');

    if (requestLine_.size() != 3)
    	return (0);

	checkMethod();
	checkURL();
	checkHTTPVersion();
    return (1);
}

int request::parse()
{
	int ret = 0;

    if (ret = parseRequestLine())
		return (ret);
	return (ret);
}

void request::read(int sockfd)
{
    char buf[LEN + 1];
    ssize_t ret = -1;

    bzero(buf, LEN);
    while ((ret = recv(sockfd, buf, LEN, 0)) > 0) // FLAG MSG_DONTWAIT ?
    {
        buf[ret] = '\0';
        requestLine_.append(buf);
    }

	if (ret == -1)
	{
		std::cerr << "error in recv()\n";
		return;
	}
	
	if (ret == 0)
	{

	}

	while (ret = parse())
	{

	}
}
