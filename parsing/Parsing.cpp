/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:17 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 02:44:02 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

typedef typename std::string           			stds;
typedef typename stds::iterator        			iterator;
typedef typename std::pair<int, stds> 			p;
typedef typename std::istreambuf_iterator<char>	ist;

Parsing::Parsing(void) : file_(stds(DEFAULT_PATH)), servers_() {}

Parsing::Parsing(stds file=DEFAULT_PATH) : file_(file), servers_() {}

Parsing::~Parsing(void) {}

void				Parsing::parseConfig(void)
{
	iterator	first;
	iterator	next;

	if (this->file_.length() < 6 || this->parseName() == false)
		throw (Parsing::ParsingException());	
	std::ifstream	file(this->file_.c_str());
	stds content((ist(file)), (ist()));
	first = content.begin();
	while (first != content.end())
	{
		this->skipWhite(&first, content.end());
		if (this->compString(&first, content.end(), stds("server")) == false)
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

Parsing::servers	Parsing::parseProps(iterator first, iterator end)
{
	Parsing::servers	server;
	std::vector<stds>	line;
	stds				tmp;
	iterator			next;

	server.host = stds("localhost");
	while (first != end)
	{
		this->skipWhite(&first, end);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end);
		if (tmp[tmp.size() - 1] != ';' && tmp.find("location") == stds::npos)
			throw (Parsing::ParsingException());
		if (tmp.find("location") != stds::npos)
		{
			line = this->splitWhitespace(stds(tmp, 0, tmp.size()));
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
			line = this->splitWhitespace(stds(tmp, 0, tmp.size() - 1));
		server = this->returnProps(server, line);
	}
	return server;
}
Parsing::location		Parsing::parseLocation(stds name, iterator first, iterator end)
{
	Parsing::location	location;
	std::vector<stds>	line;
	stds				tmp;
	
	location.name = name;
	while (first != end)
	{
		this->skipWhite(&first, end);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end);
		if (tmp[tmp.size() - 1] != ';')
			throw (Parsing::ParsingException());
		line = this->splitWhitespace(stds(tmp, 0, tmp.size() - 1));
		location = this->returnLocation(location, line);
	}
	return location;
}

Parsing::servers		Parsing::returnProps(Parsing::servers server, std::vector<stds> line)
{
	if (line.size() <= 1)
		throw (Parsing::ParsingException());
	if (this->valid(line[0], serverProps_) == false)
		throw (Parsing::ParsingException());
	if (line[0] == "listen")
	{
		if (this->to_int(line[1].c_str(), line[1].size()) != 0)
			server.port = this->to_int(line[1].c_str(), line[1].size());
		else
			throw (Parsing::ParsingException());
		if (line.size() == 3)
			server.host = line[2];
	}
	else if (line[0] == "error_page")
		if (line.size() == 3)
			server.error_pages.insert(p(this->to_int(line[1].c_str(), line[1].size()), line[2]));
	else if (line[0] == "server_name")
		for (size_t i = 0; i < line.size() - 1; i++)
			server.names.push_back(line[i + 1]);
	else if (line[0] == "root")
			server.root = line[1];
	return server;
}

Parsing::location		Parsing::returnLocation(Parsing::location location, std::vector<stds> line)
{
	iterator	first;
	iterator	second;

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
		if (this->compString(&first, line[1].end(), stds("off")))
			location.autoindex = false;
		else if (this->compString(&second, line[1].end(), stds("on")))
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
		if (this->compString(&first, line[1].end(), stds("off")))
			location.upload_enable = false;
		else if (this->compString(&second, line[1].end(), stds("on")))
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

bool				Parsing::parseName(void)
{
	if (this->file_.substr(this->file_.length() - 5) == ".conf")
		return (true);
	return (false);
}

stds				Parsing::getNextLine(iterator *first, iterator end)
{
	stds	line;

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

void				Parsing::skipWhite(iterator *first, iterator end)
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

bool				Parsing::valid(stds name, const char **valid_names)
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

bool				Parsing::compString(iterator *first, iterator end, stds src)
{
	iterator	first_one = src.begin();
	
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

iterator 			Parsing::getBrackets(iterator next, iterator end)
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

std::vector<stds>	Parsing::splitWhitespace(stds str)
{
	std::vector<stds> 	res;
	size_t				i;
	size_t				j;

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

int					Parsing::to_int(char const *s, size_t count)
{
     int		result = 0;
     size_t		i = -1 ;
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
              throw std::invalid_argument("invalid input string");
     }
     return result;
} 

const char			*Parsing::ParsingException::what() const throw()
{
	return "ConfigFile : Error Configuration";
}
