/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/10 14:07:23 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 02:00:24 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parsing.hpp"

int main()
{
	Parsing a(std::string(DEFAULT_PATH));
	a.parseConfig();
	for (size_t i = 0; i < a.getServers().size(); i++)
	{
		for (size_t x = 0; x < a.getServers()[i].names.size(); x++)
			std::cout << a.getServers()[i].names[x] << std::endl;
		std::cout << a.getServers()[i].host << std::endl;
		std::cout << a.getServers()[i].root << std::endl;
		std::cout << a.getServers()[i].port<< std::endl;
		for (size_t j = 0; j < a.getServers()[i].locations.size(); j++)
		{
			std::cout << a.getServers()[i].locations[j].name << std::endl;
			std::cout << a.getServers()[i].locations[j].root << std::endl;
			for (size_t k = 0; k < a.getServers()[i].locations[j].methods.size(); k++)
				std::cout << a.getServers()[i].locations[j].methods[k] << std::endl;
			std::cout << a.getServers()[i].locations[j].autoindex << std::endl;
			std::cout << a.getServers()[i].locations[j].index << std::endl;
			for (size_t b = 0; b < a.getServers()[i].locations[j].cgi_extension.size(); b++)
				std::cout << a.getServers()[i].locations[j].cgi_extension[b] << std::endl;
			std::cout << a.getServers()[i].locations[j].cgi_path << std::endl;
			std::cout << a.getServers()[i].locations[j].upload_path << std::endl;
		}
	}
	return (0);
}
