/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:17 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 02:23:23 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"


Parsing::Parsing(void) : file_(std::string(DEFAULT_PATH)), servers_() {}

Parsing::Parsing(std::string file=DEFAULT_PATH) : file_(file), servers_() {}

Parsing::~Parsing(void) {}

void Parsing::parseConfig(void)
{
	std::string::iterator	first;
	std::string::iterator	next;
	size_t					size = 0;

	if (this->file_.length() < 6 || this->parseName() == false)
		throw (Parsing::ParsingException());	
	std::ifstream	file(this->file_.c_str());
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	first = content.begin();
	while (first != content.end())
	{
		this->skipWhite(&first, content.end());
		if (this->compString(&first, content.end(), std::string("server")) == false)
			throw (Parsing::ParsingException());
		if (*first != '{')
			throw (Parsing::ParsingException());
		next = first;
		if (*(next = getBrackets(next, content.end())) != '}')
			throw (Parsing::ParsingException());
		this->skipWhite(&(++first), content.end());
		this->servers_.push_back(this->parseProps(first, next));
		first = next + 1;
		this->skipWhite(&first, content.end());
	}
	return;
}

Parsing::servers	Parsing::parseProps(std::string::iterator first, std::string::iterator end)
{
	Parsing::servers				server;
	std::vector<std::string>		line;
	std::string						tmp;
	std::string::iterator			next;

	server.host = std::string("localhost");
	while (first != end)
	{
		this->skipWhite(&first, end);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end);
		if (tmp[tmp.size() - 1] != ';' && tmp.find("location") == std::string::npos)
			throw (Parsing::ParsingException());
		if (tmp.find("location") != std::string::npos)
		{
			line = this->splitWhitespace(std::string(tmp, 0, tmp.size()));
			if (line.size() < 2)
				throw (Parsing::ParsingException());
			if (line[line.size() - 1] != "{" && *first != '{')
				this->skipWhite(&first, end);
			if (line[line.size() - 1] != "{" && *first != '{')
				throw (Parsing::ParsingException());
			next = first;
			if (*(next = this->getBrackets(next, end)) != '}')
				throw (Parsing::ParsingException());
			if (*first == '{')
				this->skipWhite(&(++first), end);
			else
				this->skipWhite(&first, end);
			server.locations.push_back(this->parseLocation(line[1], first, next));
			first = next + 1;
			this->skipWhite(&first, end);
			continue;
		}	
		else	
			line = this->splitWhitespace(std::string(tmp, 0, tmp.size() - 1));
		server = this->returnProps(server, line);
	}
	return server;
}
Parsing::location		Parsing::parseLocation(std::string name, std::string::iterator first, std::string::iterator end)
{
	Parsing::location				location;
	std::vector<std::string>		line;
	std::string						tmp;
	
	location.name = name;
	while (first != end)
	{
		this->skipWhite(&first, end);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end);
		if (tmp[tmp.size() - 1] != ';')
			throw (Parsing::ParsingException());
		line = this->splitWhitespace(std::string(tmp, 0, tmp.size() - 1));
		location = this->returnLocation(location, line);
	}
	return location;
}

Parsing::servers			Parsing::returnProps(Parsing::servers server, std::vector<std::string> line)
{
	if (line.size() <= 1)
		throw (Parsing::ParsingException());
	if (this->valid(line[0], serverProps_) == false)
		throw (Parsing::ParsingException());
	if (line[0] == "listen")
	{
		if (line.size() >= 2 && this->to_int(line[1].c_str(), line[1].size()) != 0)
			server.port = this->to_int(line[1].c_str(), line[1].size());
		if (line.size() == 3)
			server.host = line[2];
	}
	else if (line[0] == "error_page")
		if (line.size() == 3)
			server.error_pages.insert(std::pair<int, std::string>(this->to_int(line[1].c_str(), line[1].size()), line[2]));
	else if (line[0] == "server_name")
		for (size_t i = 0; i < line.size() - 1; i++)
			server.names.push_back(line[i + 1]);
	else if (line[0] == "root")
		if (line.size() == 2)
			server.root = line[1];
	return server;
}

Parsing::location		Parsing::returnLocation(Parsing::location location, std::vector<std::string> line)
{
	std::string::iterator	first;
	std::string::iterator	second;

	if (line.size() <= 1)
		throw (Parsing::ParsingException());
	first = line[1].begin();
	second = line[1].begin();
	if (this->valid(line[0], locationProps_) == false)
		throw (Parsing::ParsingException());
	if (line[0] == "root")
		location.root = line[1];
	else if (line[0] == "method")
		for (size_t i = 0; i < line.size() - 1; i++)
		{
			if (this->valid(line[i + 1], methods_) == false)
				throw (Parsing::ParsingException());
			location.methods.push_back(line[i + 1]);
		}
	else if (line[0] == "autoindex")
	{
		if (this->compString(&first, line[1].end(), std::string("off")))
			location.autoindex = false;
		else if (this->compString(&second, line[1].end(), std::string("on")))
			location.autoindex = true;
		else
			throw (Parsing::ParsingException());
	}
	else if (line[0] == "index")
		location.index = line[1];
	else if (line[0] == "cgi_extension")
		for (size_t i = 0; i < line.size() - 1; i++)
			location.cgi_extension.push_back(line[i + 1]);	
	else if (line[0] == "cgi_path")
		location.cgi_path = line[1];
	else if (line[0] == "upload_enable")
	{
		if (this->compString(&first, line[1].end(), std::string("off")))
			location.upload_enable = false;
		else if (this->compString(&second, line[1].end(), std::string("on")))
			location.upload_enable = true;
		else
			throw (Parsing::ParsingException());
	}	
	else if (line [0] == "upload_path")
		location.upload_path = line[1];
	/*else if (line [0] == "client_max_body_size")
	{
		
	}*/
	return (location);	
}

bool		Parsing::parseName(void)
{
	if (this->file_.substr(this->file_.length() - 5) == ".conf")
		return (true);
	return (false);
}

std::string		Parsing::getNextLine(std::string::iterator *first, std::string::iterator end)
{
	std::string	line;

	while (*first != end)
	{
		if (**first != '\n')
			line += **first;
		else if (**first == '\n')
			break;
		(*first)++;
	}
	return line;
}

void		Parsing::skipWhite(std::string::iterator *first, std::string::iterator end)
{
	while (true)
	{
		while (*first != end)
		{
			if (!(std::isspace(**first)))
				break ;
			(*first)++;
		}
		if (**first == '#')
			while ((*(*first)++) != '\n');
		if (**first != '#' && !std::isspace(**first))
			break;
	}
}

bool		Parsing::valid(std::string name, const char **valid_names)
{
	size_t i;

	i = 0;
	while (valid_names[i])
	{
		if (name == valid_names[i])
			return (true);
		++i;
	}
	return (false);
}

bool		Parsing::compString(std::string::iterator *first, std::string::iterator end, std::string src)
{
	std::string::iterator	first_one = src.begin();
	
	while ((*(*first)++) == *(first_one++))
	{
		if (first_one == src.end())
		{
			this->skipWhite(first, end);
			return (true);
		}
	}
	return (false);
}

std::string::iterator Parsing::getBrackets(std::string::iterator next, std::string::iterator end)
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

std::vector<std::string>	Parsing::splitWhitespace(std::string str)
{
	std::vector<std::string> res;
	size_t i;
	size_t j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (std::isspace(str[i]))
		{
			if (i == j)
				++j;
			else
			{
				res.push_back(std::string(str, j, i - j));
				j = i + 1;
			}
		}
		++i;
	}
	if (i != j)
		res.push_back(std::string(str, j, i - j));
	return (res);
}

int							Parsing::to_int(char const *s, size_t count)
{
     int result = 0;
     size_t i = 0 ;
     if ( s[0] == '+' || s[0] == '-' ) 
          ++i;
     while(i < count)
     {
          if ( s[i] >= '0' && s[i] <= '9' )
          {
              int value = (s[0] == '-') ? ('0' - s[i] ) : (s[i]-'0');
              result = result * 10 + value;
          }
          else
              throw std::invalid_argument("invalid input string");
          i++;
     }
     return result;
} 


const char	*Parsing::ParsingException::what() const throw()
{
	return "ConfigFile : Error Configuration";
}
