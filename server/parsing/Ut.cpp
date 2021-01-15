/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ut.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/14 02:29:45 by racohen           #+#    #+#             */
/*   Updated: 2021/01/15 07:13:48 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ut.hpp"

bool                valid(stds name, const char **valid_names)
{
    size_t i;

    i = 0;
    while (valid_names[i])
    {
        if (name == valid_names[i])
            return (true);
        i++;
    }
    return (false);
}

iterator            getBrackets(iterator next, iterator end)
{
    int begin = 1;

    while (begin != 0 && next++ != end)
    {
        if (*next == '{')
            begin++;
        if (*next == '}')
            begin--;
    }
    return (next);
}

size_t              getMcbs(std::string s)
{
    size_t size = static_cast<size_t>(to_int(s.c_str(), s.size()));

    if (s[s.size() - 1] == 'G' || s[s.size() - 1] == 'g')
        size *= 1000000000;
    else if (s[s.size() - 1] == 'M' || s[s.size() - 1] == 'm')
        size *= 1000000;
    else if (s[s.size() - 1] == 'K' || s[s.size() - 1] == 'k')
        size *= 1000;
    return (size);
}

std::vector<stds>   splitWhitespace(stds str)
{
    std::vector<stds>   res;
    size_t              i;
    size_t              j;
	bool				quotes = false;


    i = 0;
    j = 0;
    while (str[i])
    {
		if (str[i] == '\"' && quotes == true)
			quotes = false;
		else if (str[i] == '\"')
			quotes = true;
        if (std::isspace(str[i]) && quotes == false)
        {
            if (i == j)
                ++j;
            else
            {
                res.push_back(stds(str, j, i - j));
                j = i + 1;
            }
        }
        ++i;
    }
    if (i != j)
        res.push_back(stds(str, j, i - j));
    return (res);
}

int                 to_int(char const *s, size_t count)
{
     int        result = 0;
     size_t     i = -1 ;
     if ( s[0] == '+' || s[0] == '-' )
          ++i;
     while(++i < count)
     {
          if ( s[i] >= '0' && s[i] <= '9' )
          {
              int value = (s[0] == '-') ? ('0' - s[i] ) : (s[i]-'0');
              result = result * 10 + value;
          }
          else
            break;
     }
     return result;
}

std::string			to_string(int convert)
{
	std::string res;

	while (convert > 9)
	{
		res += char(convert % 10);
		convert /= 10;
	}
	res += char(convert % 10);
	return (res);
}

bool	check_port(stds line)
{
	stds::iterator first;

	for (first = line.begin(); first < line.end(); first++)
		if(*first < '0' || *first > '9')
			return (false);
	return (true);
}

bool	check_ip(stds line)
{
	stds::iterator first;
	size_t dot = 0;

	if (line == stds("localhost") || line == stds("*"))
		return (true);
	for (first = line.begin(); first < line.end();)
	{
		if (*first == '.' && dot < 3)
			dot++;
		else if((*first < '0' || *first > '9'))
			return (false);
		first++;
	}
	return (true);
}

bool	check_line(stds line)
{
	size_t	pos;
	stds	ip;
	stds	port;
	pos	= line.find(stds(":"));
	
	if (pos == line.size() - 1)
		return (check_ip(line));	
	ip = line.substr(0, pos);
	port = line.substr(pos + 1, line.size());
	if (check_ip(ip) == false || check_port(port) == false)
		return (false);
	return (true);
}
