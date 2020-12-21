#include "Utils.hpp"
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

bool isSpace(char c)
{
	if (c == ' ' || c == '\t' || c == '\v' || c == '\f' || c == '\r')
		return 1;
	return 0;
}

std::string removeSpaces(std::string & str)
{
	str.erase(std::remove_if(str.begin(), str.end(), isSpace), str.end());
	return str;
}

char ft_tolower(char i) { return std::tolower(i); }

bool boolFind(std::string const & haystack, std::string const & needle)
{
	size_t ret = haystack.find(needle);
	if (ret != std::string::npos)
		return (1);
	return (0);
}

std::string decode64(std::string const &str)
{
	size_t index, ret;
	std::string save, pack, dest, all;

	static const std::string base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	static const std::string binaryBase = "01";

	for (size_t i = 0; i < str.size(); i++)
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

static void rec_itoa(unsigned int n, std::string &str, int i)
{
	if (n >= 10)
		rec_itoa(n / 10, str, i - 1);
	if (i < 0)
		return ;
	if (n < 10)
		str[i] = n + '0';
	else
		str[i] = (n % 10) + '0';
}

std::string ft_itoa(unsigned int nb)
{
	std::string str;
	int i = 0;
	int tmp = nb;

	if (nb == 0)
		return (std::string("0"));
	while (tmp != 0)
	{
		tmp /= 10;
		i++;
	}
	rec_itoa(nb, str, i);
	return (str);
}

size_t strHex_to_int(std::string const &str)
{
	static const std::string baseHexa = "0123456789ABCDEF";
	size_t index, res = 0, pow = 0;
	
	for (std::string::const_reverse_iterator i = str.rbegin(); i != str.rend(); i++)
	{
		index = baseHexa.find(*i);
		if (index == std::string::npos)
			return (-1);

		if (!pow)
		{
			res = index;
			pow++;
		}
		else
		{
			pow = pow * 16;
			res += index * pow;
		}
	}
	return (res);
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

int getNextLine(std::string & buf, std::string & line) {
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

