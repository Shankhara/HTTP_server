/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:17 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 06:43:37 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

typedef typename std::string           			stds;
typedef typename stds::iterator        			iterator;
typedef typename std::pair<int, stds> 			p;
typedef typename std::istreambuf_iterator<char>	ist;
typedef typename Parsing::ParsingException		PpE;

stds 	content;

Parsing::Parsing(void) : file_(stds(DEFAULT_PATH)), servers_() {}

Parsing::Parsing(stds file=DEFAULT_PATH) : file_(file), servers_() {}

Parsing::~Parsing(void) {}

void				Parsing::parseConfig(void)
{
	iterator	first;
	iterator	next;

	line_ = 1;
	char_ = 1;
	if (this->file_.length() < 6 || this->parseName() == false)
		throw (PpE(this->file_, stds("File should have the .conf extension")));
	std::ifstream	file(this->file_.c_str());
	content = stds((ist(file)), (ist()));
	first = content.begin();
	while (first != content.end())
	{
		this->skipWhite(&first, content.end(), true);
		if (this->compString(&first, content.end(), stds("server")) == false)
			throw (PpE(this->file_, stds("Expected \"server\"")));
		if (*first != '{')
			throw (PpE(this->file_, stds("Expected token {")));
		next = first;
		if (*(next = getBrackets(next, content.end())) != '}')
			throw (PpE(this->file_, stds("Expected token }")));
		char_++;
		this->skipWhite(&(++first), content.end(), true);
		this->servers_.push_back(this->parseProps(first, next));
		first = next + 1;		
		this->skipWhite(&first, content.end(), true);
	}
	return;
}

Parsing::servers	Parsing::parseProps(iterator first, iterator end)
{
	Parsing::servers	server = this->getDefaultServer();
	std::vector<stds>	line;
	stds				tmp;
	iterator			next;

	while (first != end)
	{
		this->skipWhite(&first, end, true);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end, true);
		if (this->parseSemi(&tmp) == false && tmp.find("location") == stds::npos)
			throw (PpE(this->file_, stds("Expected token ;")));
		if (tmp.find("location") != stds::npos)
		{
			line = this->splitWhitespace(stds(tmp, 0, tmp.size()));
			if (line.size() < 2)
				throw (PpE(this->file_, stds("Expected at least 1 argument")));
			if (line[line.size() - 1] != "{" && *first != '{')
				this->skipWhite(&first, end, true);
			if (line[line.size() - 1] != "{" && *first != '{')
				throw (PpE(this->file_, stds("Expected token {")));
			next = first;
			if (*(next = this->getBrackets(next, end)) != '}')
				throw (PpE(this->file_, stds("Expected token }")));
			if (*first == '{')
				this->skipWhite(&(++first), end, true);
			else
				this->skipWhite(&first, end, true);
			server.locations.push_back(this->parseLocation(line[1], first, next));
			first = next + 1;
			this->skipWhite(&first, end, true);
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
	Parsing::location	location = this->getDefaultLocation();
	std::vector<stds>	line;
	stds				tmp;
	
	location.name = name;
	while (first != end)
	{
		this->skipWhite(&first, end, true);
		tmp = getNextLine(&first, end);
		this->skipWhite(&first, end, true);
		if (tmp[tmp.size() - 1] != ';')
			throw (PpE(this->file_, stds("Expected token ;")));
		line = this->splitWhitespace(stds(tmp, 0, tmp.size() - 1));
		location = this->returnLocation(location, line);
	}
	return location;
}

Parsing::servers		Parsing::returnProps(Parsing::servers server, std::vector<stds> line)
{
	if (line.size() <= 1)
		throw (PpE(this->file_, stds("Expected at least 1 argument")));
	if (this->valid(line[0], serverProps_) == false)
		throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[0])));
	if (line[0] == "listen")
	{
		if (this->to_int(line[1].c_str(), line[1].size()) == 0)
			throw (PpE(this->file_, stds("Port can't be 0")));
		server.port = this->to_int(line[1].c_str(), line[1].size());
		if (line.size() == 3)
			server.host = line[2];
	}
	else if (line[0] == "error_page")
		if (line.size() == 3)
			server.error_pages[this->to_int(line[1].c_str(), line[1].size())] = line[2];
	else if (line[0] == "server_name")
		for (size_t i = 0; i < line.size() - 1; i++)
			server.names.push_back(line[i + 1]);
	else if (line[0] == "root")
			server.root = line[1];
	else if (line[0] == "access_log")
		if (line.size() == 3)
			server.access_log.insert(std::pair<stds, stds>(line[1], line[2]));
	return server;
}

Parsing::location		Parsing::returnLocation(Parsing::location location, std::vector<stds> line)
{
	iterator	first;
	iterator	second;

	if (line.size() <= 1)
		throw (PpE(this->file_, stds("Excepted at least 1 argument")));
	first = line[1].begin();
	second = line[1].begin();
	if (this->valid(line[0], locationProps_) == false)
		throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[0])));
	if (line[0] == "root")
		location.root = line[1];
	else if (line[0] == "method")
		for (size_t i = 0; i < line.size() - 1; i++)
		{
			if (this->valid(line[i + 1], methods_) == false)
				throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[i + 1])));
			location.methods.push_back(line[i + 1]);
		}
	else if (line[0] == "autoindex")
	{
		if (this->compString(&first, line[1].end(), stds("off")))
			location.autoindex = false;
		else if (this->compString(&second, line[1].end(), stds("on")))
			location.autoindex = true;
		else
			throw (PpE(this->file_, stds("Value can be set with \"on\" or \"off\" only")));
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
			throw (PpE(this->file_, stds("Value can be set with \"on\" or \"off\" only")));
	}	
	else if (line [0] == "upload_path")
		location.upload_path = line[1];
	else if (line [0] == "client_max_body_size")
		location.client_max_body_size = this->getMcbs(line[1]);
	return (location);	
}

bool				Parsing::parseName(void)
{
	if (this->file_.substr(this->file_.length() - 5) == ".conf")
		return (true);
	return (false);
}

bool				Parsing::parseSemi(stds *src)
{
	iterator first = src->begin();
	iterator end = src->end();

	stds	*tmp = new stds();
	bool	seen = false;
	while (first != end)
	{
		if (*first == ';' && seen == false)
			seen = true;
		else if (*first == ';' && seen == true)
			return (false);
		else if (*first == '#' || *first == '}')
			break;
		char_++;
		first++;	
	}
	first = src->begin();
	while (first != end)
	{
		(*tmp) += *first;
		if (*first == ';')
			break;
		first++;
	}
	src = tmp;
	return (true);
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
		char_++;
		(*first)++;
	}
	return line;
}

Parsing::servers	Parsing::getDefaultServer()
{
	Parsing::servers server;

	server.port = 80;
	server.host = "127.0.0.1";
	server.root = "";
	server.error_pages = std::map<int, stds>();
	return (server);
}

Parsing::location	Parsing::getDefaultLocation()
{
	Parsing::location location;

	location.name = "/";
	location.root = "";
	location.index = "";
	location.autoindex = false;
	location.cgi_path = "";
	location.upload_enable = false;
	location.upload_path = "";
	location.client_max_body_size = 1048576;
	return (location);
}

void				Parsing::skipWhite(iterator *first, iterator end, bool inc)
{
	while (true)
	{
		while (*first != end)
		{
			if (!(std::isspace(**first)))
				break ;
			if (**first == '\n' && inc)
			{
				line_++;
				char_ = 0;
			}
			char_++;
			(*first)++;
		}
		if (**first == '#')
		{
			while ((*(*first)++) != '\n' && *first != end);
			line_++;
			char_ = 1;
		}
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
			this->skipWhite(first, end, false);
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

size_t				Parsing::getMcbs(std::string s)
{
	size_t size = static_cast<size_t>(this->to_int(s.c_str(), s.size()));

	if (s[s.size() - 1] == 'G' || s[s.size() - 1] == 'g')
		size *= 1000000000;	
	else if (s[s.size() - 1] == 'M' || s[s.size() - 1] == 'm')
		size *= 1000000;	
	else if (s[s.size() - 1] == 'K' || s[s.size() - 1] == 'k')
		size *= 1000;	
	return (size);
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
			break;
     }
     return result;
} 

