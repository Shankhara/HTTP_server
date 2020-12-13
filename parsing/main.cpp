/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 14:07:23 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 03:57:06 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

typedef typename std::map<int, std::string>::iterator ite;

int main()
{
	Parsing a(std::string(DEFAULT_PATH));
	a.parseConfig();
	for (size_t i = 0; i < a.getServers().size(); i++)
	{
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << " =================== " << std::endl;
		std::cout << " 	  SERVER : " << i << std::endl;
		std::cout << " =================== " << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		for (size_t x = 0; x < a.getServers()[i].names.size(); x++)
			std::cout << "NAME " << x << " : " << a.getServers()[i].names[x] << std::endl;
		std::cout << "HOST : " << a.getServers()[i].host << std::endl;
		std::cout << "ROOT : " << a.getServers()[i].root << std::endl;
		std::cout << "PORT : " << a.getServers()[i].port << std::endl;
		if (a.getServers()[i].error_pages.empty() == false)
		{
			std::map<int, std::string>::iterator it = a.getServers()[i].error_pages.begin();
			std::cout << "ERROR_PAGE : " << std::endl;
			std::cout << "	CODE : " << it->first << std::endl;
			std::cout << "	PAGE : " << it->second << std::endl;
			std::cout << std::endl;
		}
		for (std::map<std::string, std::string>::iterator it = a.getServers()[i].access_log.begin(); it != a.getServers()[i].access_log.end(); it++)
		{
			std::cout << "ACCESS_LOG : " << std::endl;
			std::cout << "	DOMAIN : " << it->first << std::endl;
			std::cout << "	NAME : " << it->second << std::endl;
			std::cout << std::endl;
		}
		std::cout << "LOCATION : " << std::endl;
		for (size_t j = 0; j < a.getServers()[i].locations.size(); j++)
		{
			std::cout << std::endl << "	==========  " << std::endl;
			std::cout << "	LOCATION : " << a.getServers()[i].locations[j].name << std::endl;
			std::cout << "	ROOT : " << a.getServers()[i].locations[j].root << std::endl;
			std::cout << "	METHODS : ";
			for (size_t k = 0; k < a.getServers()[i].locations[j].methods.size(); k++)
				std::cout << a.getServers()[i].locations[j].methods[k] << " ";
			std::cout << std::endl;
			std::cout << "	AUTOINDEX : " << a.getServers()[i].locations[j].autoindex << std::endl;
			std::cout << "	INDEX : " << a.getServers()[i].locations[j].index << std::endl;
			std::cout << "	CGI_EXTENSION : ";
			for (size_t b = 0; b < a.getServers()[i].locations[j].cgi_extension.size(); b++)
				std::cout << a.getServers()[i].locations[j].cgi_extension[b];
			std::cout << std::endl;
			std::cout << "	CGI_PATH : " << a.getServers()[i].locations[j].cgi_path << std::endl;
			std::cout << "	UPLOAD_PATH : " << a.getServers()[i].locations[j].upload_path << std::endl;
			std::cout << "	CLIENT_MAX_BODY_SIZE : " << a.getServers()[i].locations[j].client_max_body_size << std::endl;
		}
	}
	return (0);
}
