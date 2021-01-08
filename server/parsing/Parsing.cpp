/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/09 16:15:17 by racohen           #+#    #+#             */
/*   Updated: 2021/01/04 13:25:50 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Parsing.hpp"


Parsing *Parsing::instance_ = 0;

typedef std::string           			stds;
typedef stds::iterator        			iterator;
typedef std::pair<int, stds> 			pi;
typedef std::pair<stds, stds> 			ps;
typedef std::istreambuf_iterator<char>	ist;
typedef Parsing::ParsingException		PpE;

stds 	content;

Parsing::Parsing(void) : file_(stds(DEFAULT_PATH)), servers_() {}

Parsing::Parsing(stds file) : file_(file), servers_() {}

Parsing::~Parsing(void) {}

void				Parsing::parseConfig(void)
{
	iterator	first;
	iterator	next;

	line_ = 1;
	char_ = 1;
	if (this->file_.length() < 6 ||
		this->file_.substr(this->file_.length() - 5).compare(stds(".conf")) != 0)
			throw (PpE(this->file_, stds("File should have the .conf extension")));
	std::ifstream	file(this->file_.c_str());
	if (file.is_open() == false)
		throw (PpE(this->file_, stds("File doesn't exist")));
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
	if (this->servers_.empty())
		throw (PpE(this->file_, stds("No server defined")));
	return;
}

Parsing::server	Parsing::parseProps(iterator first, iterator end)
{
	Parsing::server		server = this->getDefaultServer();
	std::vector<stds>	line;
	std::vector<int>	prop = this->getTableDef();
	stds				*tmp;
	iterator			next;

	while (first != end)
	{
		this->skipWhite(&first, end, true);
		tmp = new stds(getNextLine(&first, end));
		this->skipWhite(&first, end, true);
		if (this->parseSemi(tmp) == false && tmp->find("location") == stds::npos)
			throw (PpE(this->file_, stds("Expected token ;")));
		if (tmp->find("location") != stds::npos)
		{
			line = splitWhitespace(stds(*tmp, 0, tmp->size()));
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
			for (size_t i = 0; i < server.locations.size() - 1; i++)
				if (server.locations[i].name == line[1])
					throw (PpE(this->file_, stds(" duplicated locations ")));
			first = next + 1;
			this->skipWhite(&first, end, true);
			delete tmp;
			continue;
		}	
		else	
			line = splitWhitespace(stds(*tmp, 0, tmp->size() - 1));
		delete tmp;
		server = this->returnProps(server, line, &prop);
	}
	return server;
}
Parsing::location		Parsing::parseLocation(stds name, iterator first, iterator end)
{
	Parsing::location	location = this->getDefaultLocation();
	std::vector<stds>	line;
	std::vector<int>	prop = this->getTableDef();
	stds				*tmp;
	
	location.name = name;
	while (first != end)
	{
		this->skipWhite(&first, end, true);
		tmp = new stds(getNextLine(&first, end));
		this->skipWhite(&first, end, true);
		if (this->parseSemi(tmp) == false)
			throw (PpE(this->file_, stds("Expected token ;")));
		line = splitWhitespace(stds(*tmp, 0, tmp->size() - 1));
		location = this->returnLocation(location, line, &prop);
		delete tmp;
	}
	return location;
}

Parsing::server		Parsing::returnProps(Parsing::server server, std::vector<stds> line, std::vector<int> *prop)
{
	stds	listen;
	size_t	pos;

	if (line.size() <= 1)
		throw (PpE(this->file_, stds("Expected at least 1 argument")));
	if (valid(line[0], serverProps_) == false)
		throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[0])));
	if (line[0] == "listen")
	{
		if ((*prop)[0] == 0)
			(*prop)[0] = 1;
		else
			throw (PpE(this->file_, stds(" listen duplicated ")));
		pos = line[1].find(stds(":"));
		if (pos != stds::npos)
		{
			listen = line[1].substr(pos + 1, line[1].size());		
			if (to_int(listen.c_str(), listen.size()) < 0 || to_int(listen.c_str(), listen.size()) > 65535)
				throw (PpE(this->file_, stds("Port can't be 0")));
			server.port = to_int(listen.c_str(), listen.size());
		}
		if (pos != stds::npos)
			server.host = line[1].substr(0, pos);
		else
			server.host = line[1];
		if (server.host == stds("*"))
			server.host = stds("0.0.0.0");
		if (server.host.size() < 6)	
			throw (PpE(this->file_, stds("invalid host")));
	}
	else if (line[0] == "error_page")
	{
		if ((*prop)[1] == 0)
			(*prop)[1] = 1;
		else
			throw (PpE(this->file_, stds(" error_page duplicated ")));
		if (line.size() == 3)
			server.error_pages.push_back(pi(to_int(line[1].c_str(), line[1].size()), line[2]));
		else
			throw (PpE(this->file_, stds("error_page need at least 2 arguments")));
	}
	else if (line[0] == "server_name")
	{
		if ((*prop)[2] == 0)
			(*prop)[2] = 1;
		else
			throw (PpE(this->file_, stds(" server_name duplicated ")));
		for (size_t k = 0; k < line.size() - 1; k++)
			for (size_t i = 0; i < this->getServers().size(); i++)
				for (size_t j = 0; j < this->getServers()[i].names.size(); j++)
					if (line[k + 1] == this->getServers()[i].names[j])
						return server;
					//	throw (PpE(this->file_, stds(" duplicated server_name ")));
		for (size_t i = 0; i < line.size() - 1; i++)
			server.names.push_back(line[i + 1]);
	}
	else if (line[0] == "root")
	{
		if ((*prop)[3] == 0)
			(*prop)[3] = 1;
		else
			throw (PpE(this->file_, stds(" root duplicated ")));
		for (size_t i = 0; i < this->getServers().size(); i++)
			if (this->getServers()[i].root == line[1])
				throw (PpE(this->file_, stds(" duplicated root in server")));	
		if (line[1][0] != '/')
			throw (PpE(this->file_, stds("root need absolute path")));
		server.root = line[1];
	}
	else if (line [0] == "client_max_body_size")
	{
		if ((*prop)[4] == 0)
			(*prop)[4] = 1;
		else
			throw (PpE(this->file_, stds(" client_max_body_size duplicated ")));
		server.client_max_body_size = getMcbs(line[1]);
	}
	return server;
}

Parsing::location		Parsing::returnLocation(Parsing::location location, std::vector<stds> line, std::vector<int> *prop)
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
	{
		if ((*prop)[0] == 0)
			(*prop)[0] = 1;
		else
			throw (PpE(this->file_, stds(" root duplicated ")));	
		if (line[1][0] != '/')
			throw (PpE(this->file_, stds("root need absolute path")));
		location.root = line[1];
	}
	else if (line[0] == "method")
	{
		if ((*prop)[1] == 0)
			(*prop)[1] = 1;
		else
			throw (PpE(this->file_, stds(" method duplicated ")));	
		for (size_t i = 0; i < line.size() - 1; i++)
		{
			if (valid(line[i + 1], methods_) == false)
				throw (PpE(this->file_, stds(stds("Unknown identifier ") + line[i + 1])));
			location.methods.push_back(line[i + 1]);
		}
	}
	else if (line[0] == "autoindex")
	{
		if ((*prop)[2] == 0)
			(*prop)[2] = 1;
		else
			throw (PpE(this->file_, stds(" autoindex duplicated ")));	
		if (compString(&first, line[1].end(), stds("off")))
			location.autoindex = false;
		else if (compString(&second, line[1].end(), stds("on")))
			location.autoindex = true;
		else
			throw (PpE(this->file_, stds("Value can be set with \"on\" or \"off\" only")));
	}
	else if (line[0] == "index")
	{
		if ((*prop)[3] == 0)
			(*prop)[3] = 1;
		else
			throw (PpE(this->file_, stds(" root duplicated ")));	
		location.index = line[1];
	}
	else if (line[0] == "cgi_extension")
	{	
		if ((*prop)[4] == 0)
			(*prop)[4] = 1;
		else
			throw (PpE(this->file_, stds(" cgi_extension duplicated ")));
		for (size_t i = 0; i < line.size() - 1; i++)
			location.cgi_extension.push_back(line[i + 1]);	
	}
	else if (line[0] == "cgi_path")
	{
		if ((*prop)[5] == 0)
			(*prop)[5] = 1;
		else
			throw (PpE(this->file_, stds(" cgi_path duplicated ")));	
		location.cgi_path = line[1];
	}
	else if (line[0] == "upload_enable")
	{
		if ((*prop)[6] == 0)
			(*prop)[6] = 1;
		else
			throw (PpE(this->file_, stds(" upload_enable duplicated ")));
		if (compString(&first, line[1].end(), stds("off")))
			location.upload_enable = false;
		else if (compString(&second, line[1].end(), stds("on")))
			location.upload_enable = true;
		else
			throw (PpE(this->file_, stds("Value can be set with \"on\" or \"off\" only")));
	}	
	else if (line [0] == "upload_path")
	{	
		if ((*prop)[7] == 0)
			(*prop)[7] = 1;
		else
			throw (PpE(this->file_, stds(" upload_path duplicated ")));	
		location.upload_path = line[1];
	}
	else if (line[0] == "client_max_body_size")
	{
		if ((*prop)[8] == 0)
			(*prop)[8] = 1;
		else
			throw (PpE(this->file_, stds(" root duplicated ")));
		location.client_max_body_size = getMcbs(line[1]);
	}
	else if (line[0] == "auth_basic")
	{
		if ((*prop)[9] == 0)
			(*prop)[9] = 1;
		else
			throw (PpE(this->file_, stds(" auth_basic duplicated ")));	
		location.auth_basic = line[1];
	}
	else if (line[0] == "auth_basic_user_file")
	{
		if ((*prop)[10] == 0)
			(*prop)[10] = 1;
		else
			throw (PpE(this->file_, stds(" auth_basic_user_file duplicated ")));	
		location.auth_basic_user_file = line[1];
	}
	return (location);	
}

bool				Parsing::parseSemi(stds *src)
{
	iterator	first = src->begin();
	iterator	end = src->end();
	bool		seen = false;
	size_t		i = 0;

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
	if (seen == false)
		return (false);
	first = src->begin();
	while (first != end)
	{
		i++;
		if (*first == ';')
			break;
		first++;
	}
	*src = src->substr(0, i);
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

Parsing::server		Parsing::getDefaultServer()
{
	Parsing::server server;

	server.port = 80;
	server.host = "127.0.0.1";
	server.root = "";
	server.client_max_body_size = 536870912;
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
	location.auth_basic = "";
	location.auth_basic_user_file = "";
	location.upload_enable = false;
	location.upload_path = "";
	location.client_max_body_size = 536870912;
	return (location);
}

std::vector<int>	Parsing::getTableDef()
{
	std::vector<int>	vec(11);

	for (size_t i = 0; i < vec.size(); i++)
		vec[i] = 0;	
	return (vec);
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


Parsing *Parsing::getInstance()
{
	if (instance_ == 0)
		instance_ = new Parsing();
	return instance_;
}

void Parsing::setFile(const stds &file) {
	file_ = file;
}
