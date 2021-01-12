#include "Mime.hpp"
#include "Log.hpp"

Mime * Mime::instance_ = 0;

Mime * Mime::getInstance()
{
	if (!instance_)
		new Mime();
	return instance_;
}

void Mime::releaseInstance()
{
	if (instance_)
	{
		delete instance_;
		instance_ = 0;
	}
}

Mime::Mime()
{
	parseMimeFile();
	instance_ = this;
}

Mime::~Mime() { }

void Mime::parseMimeFile()
{
	char buf[BUFFER_SIZE + 1];
	std::string str, line;
	size_t nbytes, endType, startExt;

	int fd = open("/etc/mime.types", O_RDONLY);
	if (fd == -1)
		return;

	while ((nbytes = read(fd, buf, BUFFER_SIZE)) > 0)
		str.append(buf, nbytes);

	while (getNextLine(str, line) > 0)
	{
		if (line[0] == '#' || line.empty())
			continue;
		for (endType = 0; line[endType] > 32 && line[endType] < 127; endType++) {}
		startExt = line.find_first_not_of(line[endType], endType);
		if (startExt == std::string::npos)
			continue;
		mime_.push_back(line.substr(0, endType));
		extensions_.push_back(explode(line.substr(startExt), ' '));
	}
	close(fd);
}

std::string Mime::getExtension(const std::string & filename) const
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
	size_t start = param.rfind("/");
	if (start != std::string::npos)
		return param.substr(start + 1);
	return param;
}

std::string Mime::getContentType(std::string & param) const
{
	std::string ext = getExtension(getFilename(param));
    if(!ext.empty())
    {
        for(size_t i = 0; i < extensions_.size(); i++)
        {
            for(size_t j = 0; j < extensions_[i].size(); j++)
            {
                if(extensions_[i][j].compare(ext) == 0)
                    return mime_[i];
            }
        }
    }
    return "application/octet-stream";
}
