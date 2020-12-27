#include "Mime.hpp"

Mime::Mime()
{
	parseMimeFile();
}

Mime::~Mime()
{

}

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
}

std::string Mime::getExtension(std::string & filename) const
{
	size_t start, end;
	size_t dead = std::string::npos;
	std::string res;

	start = filename.find('.', 0);
	end = filename.find('.', start);
	if (start == dead)
		return (res); // no extension
	return filename.substr(start + 1, end - 1);
}

std::string Mime::getContentType(std::string & filename) const
{
	std::string ext = getExtension(filename);
	std::cout << "a: " << ext << std::endl;

	for (size_t i = 0; i < extensions_.size(); i++)
	{
		for (size_t j = 0; j < extensions_[i].size(); j++)
		{
			if (extensions_[i][j].compare(ext) == 0)
				return mime_[i];
		}
	}
	return "text/plain";
}
