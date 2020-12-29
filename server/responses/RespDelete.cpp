#include "RespDelete.hpp"

RespDelete::RespDelete(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	setFilePath();
}

RespDelete::~RespDelete() { }

void RespDelete::del_it_()
{
    struct stat statbuf;

    int ret = stat(filePath_.c_str(), &statbuf);
	if (!ret)
		std::cout << S_ISDIR(statbuf.st_mode) << std::endl;
}

int RespDelete::readResponse()
{
	return 0;
}
