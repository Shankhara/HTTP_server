#pragma once 

#include <cstring>
#include <vector>
#include <string>

char ft_tolower(char i) { return std::tolower(i); }

std::string decode64(std::string const &str)
{
	size_t len = str.size(), index, ret;
	std::string save, pack, dest, all;

    static const std::string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    static const std::string binaryBase = "01";

	for (size_t i = 0; i < len; i++)
    {
		index = base64.find(str[i]);
		if (index == std::string::npos)
			return (0);

		for (int i = 0; i < 6; i++)
    	{
    	    pack.insert(pack.begin(), index % 2 + '0');
    	    index /= 2;
    	}
        all.append(pack);
		pack.clear();
    }

	size_t packCount = all.size() / 8;

	for (size_t i = 0; i < packCount; i++)
	{
		ret = 0;
    	for (size_t i = 0; i < 8; i++)
       		ret = ret * 2 + binaryBase.find(all[i]);

        dest.push_back(ret);
        all.erase(0, 8);
    }
    return (dest);
}

int ft_atoi(std::string const &str)
{
	int res = 0;

	for (int i = 0; str[i] != '\0'; i++)
        res = res * 10 + str[i] - '0';
    return res;
}

size_t strHex_to_int(std::string const &str)
{ 
	size_t x;   
	std::stringstream ss;

	ss << std::hex << str;
	ss >> x;
	return x;
}

std::vector<std::string> explode(const std::string & str, const char & delim)
{
	size_t start, end = 0;
	std::vector<std::string> res;

	while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
	{
		end = str.find(delim, start);
		res.push_back(str.substr(start, end - start));
	}
	return (res);
}

int getNextLine(std::string & buf, std::string & line)
{
	size_t pos;

	pos = buf.find("\n");

	if (pos == std::string::npos)
		return (-1);

	line = std::string(buf, 0, pos++);
	buf = buf.substr(pos);

	if (pos != std::string::npos)
		return (1);

	return (0);
}

