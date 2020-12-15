/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:17 by racohen           #+#    #+#             */
/*   Updated: 2020/12/14 22:35:56 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"

typedef typename std::string           			stds;
typedef typename stds::iterator        			iterator;
typedef typename std::pair<int, stds> 			pi;
typedef typename std::pair<stds, stds> 			ps;
typedef typename std::istreambuf_iterator<char>	ist;
typedef typename Parsing::ParsingException		PpE;

stds 	content;

Parsing::Parsing(void) : file_(stds(DEFAULT_PATH)), servers_() {}

Parsing::Parsing(stds file=DEFAULT_PATH) : file_(file), servers_() {}

Parsing::~Parsing(void) {}

void				Parsing::parseConfig(void)
{
	std::__cxx11::basic_string<char>	*ptr = NULL;
	iterator	first;
	iterator	next;

	line_ = 1;
	char_ = 1;
	if (this->file_.length() < 6)
	{
		*ptr = this->file_.substr(this->file_.length() - 5);
		if (*ptr != ".conf")
			throw (PpE(this->file_, stds("File should have the .conf extension")));
	}
	free(ptr);
	std::ifstream	file(this->file_.c_str());
	content = stds((ist(file)), (ist()));
	first = content.begin();
	file.close();
	while (first != content.end())
	{
		this->skipWhite(&first, content.end(), true);
		if (compString(&first, content.end(), stds("server")) == false)
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
			line = splitWhitespace(stds(tmp, 0, tmp.size()));
			if (line.size() < 2)
				throw (PpE(this->file_, stds("Expected at least 1 argument")));
			if (line[line.size() - 1] != "{" && *first != '{')
				this->skipWhite(&first, end, true);
			if (line[line.size() - 1] != "{" && *first != '{')
				throw (PpE(this->file_, stds("Expected token {")));
			next = first;
			if (*(next = getBrackets(next, end)) != '}')
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
			line = splitWhitespace(stds(tmp, 0, tmp.size() - 1));
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
		line = splitWhitespace(stds(tmp, 0, tmp.size() - 1));
		location = this->returnLocation(location, line);
	}
	return location;
}

Parsing::servers		Parsing::returnProps(Parsing::servers server, std::vector<stds> line)
{
	if (line.size() <= 1)
		throw (PpE(this->file_, stds("Expected at least 1 argument")));
	if (valid(line[0], serverProps_) == false)
		throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[0])));
	if (line[0] == "listen")
	{
		if (to_int(line[1].c_str(), line[1].size()) == 0)
			throw (PpE(this->file_, stds("Port can't be 0")));
		server.port = to_int(line[1].c_str(), line[1].size());
		if (line.size() == 3)
			server.host = line[2];
	}
	else if (line[0] == "error_page")
	{
		if (line.size() == 3)
			server.error_pages.push_back(pi(to_int(line[1].c_str(), line[1].size()), line[2]));
		else
			throw (PpE(this->file_, stds("error_page need at least 2 arguments")));
	}
	else if (line[0] == "server_name")
		for (size_t i = 0; i < line.size() - 1; i++)
			server.names.push_back(line[i + 1]);
	else if (line[0] == "root")
			server.root = line[1];
	else if (line[0] == "access_log")
	{
		if (line.size() == 3)
			server.access_log.push_back(ps(line[1], line[2]));
		else
			throw (PpE(this->file_, stds("access_log need at least 2 arguments")));
	}
	else if (line [0] == "client_max_body_size")
		server.client_max_body_size = getMcbs(line[1]);
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
	if (valid(line[0], locationProps_) == false)
		throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[0])));
	if (line[0] == "root")
		location.root = line[1];
	else if (line[0] == "method")
		for (size_t i = 0; i < line.size() - 1; i++)
		{
			if (valid(line[i + 1], methods_) == false)
				throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[i + 1])));
			location.methods.push_back(line[i + 1]);
		}
	else if (line[0] == "autoindex")
	{
		if (compString(&first, line[1].end(), stds("off")))
			location.autoindex = false;
		else if (compString(&second, line[1].end(), stds("on")))
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
		if (compString(&first, line[1].end(), stds("off")))
			location.upload_enable = false;
		else if (compString(&second, line[1].end(), stds("on")))
			location.upload_enable = true;
		else
			throw (PpE(this->file_, stds("Value can be set with \"on\" or \"off\" only")));
	}	
	else if (line [0] == "upload_path")
		location.upload_path = line[1];
	else if (line [0] == "client_max_body_size")
		location.client_max_body_size = getMcbs(line[1]);
	return (location);	
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
	server.client_max_body_size = 1048576;
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

bool              	Parsing::compString(iterator *first, iterator end, stds src) 
{ 
    iterator    first_one = src.begin(); 
 
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
