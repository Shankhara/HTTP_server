#include "RespDelete.hpp"

RespDelete::RespDelete(const Request &r, char buf[], unsigned int bufSize) : RespFile(r, buf, bufSize) { }

RespDelete::~RespDelete() { }

int RespDelete::delDir_(std::string & param)
{
	const char *c_param = param.c_str();

    DIR *dir = opendir(c_param);
	if (!dir)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
		return EXIT_FAILURE;
    }

    struct dirent *entry;
	std::string name, concat_path;

    while ((entry = readdir(dir)))
	{
		name = entry->d_name;
        if (name == "." || name == "..")
            continue;

		concat_path += param;
		concat_path += "/";
		concat_path += name;
        if (entry->d_type == DT_DIR)
        {
            delDir_(concat_path);
        }
        else
        {
            unlink(concat_path.c_str());
        }
    }
    if (rmdir(c_param) == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 500;
		return EXIT_FAILURE;
    }

	closedir(dir);
	return EXIT_SUCCESS;
}

int RespDelete::delResource_()
{
    struct stat statbuf = {};
 
    int ret = stat(filePath_.c_str(), &statbuf);
	if (ret == -1)
	{
		Log().Get(logDEBUG) << __FUNCTION__  << " unable to open: " << strerror(errno);
		statusCode_ = 404;
		return ret;
	}
	if (!S_ISDIR(statbuf.st_mode))
		return unlink(filePath_.c_str());

	return delDir_(filePath_);
}

void RespDelete::makeResponse_()
{
    if (!headersBuilt_) {
        writeStatusLine_(statusCode_);
        writeHeadersEnd_();
    }
}

int RespDelete::readResponse()
{
	nbytes_ = 0;

	if (!delResource_())
		statusCode_ = 204;

	makeResponse_();

	return nbytes_;
}
