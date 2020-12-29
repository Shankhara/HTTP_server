#include "RespDelete.hpp"

RespDelete::RespDelete(const Request &r, char buf[], unsigned int bufSize) : Response(r, buf, bufSize)
{
	setFilePath();
}

RespDelete::~RespDelete() { }

int RespDelete::delDir_(char *param)
{
    DIR *dir = opendir(param);
	if (!dir)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
    }

    struct dirent *entry;
	char slash[2] = "/";
	char currentDir[2] = ".";
	char prevDir[3] = "..";

    while ((entry = readdir(dir)))
	{
        if (ft_strcmp(entry->d_name, currentDir) || ft_strcmp(entry->d_name, prevDir))
            continue;

		char * tmp = ft_strcat(ft_strcat(param, slash), entry->d_name);
        if (entry->d_type == DT_DIR)
            delDir_(tmp);
        else
			unlink(tmp);
		free(tmp);
    }
    if (!rmdir(param) && !closedir(dir))
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}

int RespDelete::reachResource_()
{
    struct stat statbuf;
 
    int ret = stat(filePath_.c_str(), &statbuf);
	if (ret == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		return -1;
	}

	if (!ret && !S_ISDIR(statbuf.st_mode))
		return 0;
	return 1;
}

void RespDelete::makeResponse_()
{
	writeStatusLine_(statusCode_);
	writeHeadersEnd_();
}

int RespDelete::readResponse()
{
	int ret = reachResource_();

	if (!ret && !unlink(filePath_.c_str()))
		statusCode_ = 204;
	
	if (ret == 1 && delDir_(&filePath_[0]))
		statusCode_ = 204;
	
	if (ret == -1)
		statusCode_ = 404;

	makeResponse_();

	return nbytes_;
}
