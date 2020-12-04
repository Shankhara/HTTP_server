#pragma once

#include <cstring>
#include <vector>
#include <string>

const std::vector<std::string> explode(const std::string & str, const char & delim)
{
	std::string buf;
	std::vector<std::string> res;
	
	for (char i:str)
	{
		if (i != delim)
			buf += i;
		else if (i == delim && !buf.empty())
		{
			res.push_back(buf);
			buf.clear();
		}
	}
	if (!buf.empty())
		res.push_back(buf);
	return res;
}

int getNextLine(std::string & buf, std::string & line)
{
	size_t pos;

	pos = buf.find("\n");
	line = std::string(buf, 0, pos++);
	buf = buf.substr(pos);
	if (pos != std::string::npos)
		return (1);
	return (0);
}
