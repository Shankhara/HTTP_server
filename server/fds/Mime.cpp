#include "Mime.hpp"

Mime * Mime::instance_ = 0;

Mime * Mime::getInstance()
{
	if (!instance_)
		new Mime();
	return instance_;
}

Mime::Mime()
{
	openFile_();
	instance_ = this;
}

Mime::~Mime() { }

void Mime::parseMimeFile()
{
	char buf[BUFFER_SIZE + 1];
	std::string str, line;
	size_t nbytes, endType, startExt;

	if (fd_ < 0)
		return ;

	while ((nbytes = read(fd_, buf, BUFFER_SIZE)) > 0)
	{
		str.append(buf, nbytes);
		if (str.size() > 512 * 1024)
		{
			Log::get(logERROR) << "Unable to read: " << MIME_FILE << " Unexpected size: " << str.size() << std::endl;
			return ;
		}
	}
	if (nbytes < 0)
	{
		Log::get(logERROR) << "Unable to read: " << MIME_FILE << " " << strerror(errno) << std::endl;
		return ;
	}

	while (getNextLine(str, line) > 0)
	{
		if (line[0] == '#' || line.empty())
			continue;
		for (endType = 0; line[endType] > 32 && line[endType] < 127; endType++) {}
		startExt = line.find_first_not_of(line[endType], endType);
		if (startExt == std::string::npos)
			continue;
        std::vector<std::string> tmp = explode(line.substr(startExt), ' ');
        std::string type = line.substr(0, endType);
        for(size_t i = 0; i < tmp.size(); ++i)
			mimeTypes_[tmp[i]] = type;
	}
	Log::get(logINFO) << MIME_FILE << " loaded: " << mimeTypes_.size() << " entries." << std::endl;
	Server::getInstance()->unwatch(fd_);
}

std::string Mime::getExtension(const std::string & filename)
{
	size_t start;
	size_t dead = std::string::npos;

	start = filename.find_last_of('.');
	if (start == dead)
		return (filename);
	return filename.substr(start + 1, filename.size() - start);
}

std::string Mime::getFilename(std::string & param) const
{
	size_t start = param.rfind('/');
	if (start != std::string::npos)
		return param.substr(start + 1);
	return param;
}

std::string Mime::getContentType(std::string & param)
{
	std::string ext = getExtension(getFilename(param));
    if(!ext.empty())
	{
		if (mimeTypes_[ext].empty())
			return "application/octet-stream";
		return mimeTypes_[ext];
	}
    return "application/octet-stream";
}

void Mime::onEvent() {
	parseMimeFile();
}

void Mime::openFile_() {
	fd_ = open(MIME_FILE, O_RDONLY);
	if (fd_ < 0)
	{
		Log::get(logERROR) << " Unable to open: " << MIME_FILE << " : " << strerror(errno) << std::endl;
	}
}
