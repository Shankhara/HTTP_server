/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 16:17:34 by racohen           #+#    #+#             */
/*   Updated: 2020/12/12 20:33:18 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <iostream>
# include <list>
# include <map>
# include <unistd.h>
# include <fcntl.h>
# include <iostream>
# include <cstdlib>
# include <vector>
# include <sstream>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# define	DEFAULT_PATH "./test/nginx.conf"

static const char 		*serverProps_[] = { "listen",
											"server_name",
											"error_page",
											"root",
											"access_log",
											0 };
										
static const char		*methods_[] = { "GET",
										"HEAD",
										"POST",
										"PUT",
										"DELETE",
										"CONNECT",
										"OPTIONS",
										"TRACE",
										0 };

static const char		*locationProps_[] = {	"method",
												"root",
												"autoindex",
												"index",
												"cgi_extension",
												"cgi_path",
												"upload_enable",
												"upload_path",
												"client_max_body_size",
												0 };


class Parsing
{
	private :

		struct location
		{
			std::string					name;
			std::string					root;
			std::vector<std::string>	methods;
			bool						autoindex;
			std::string					index;
			std::vector<std::string>	cgi_extension;
			std::string					cgi_path;
			bool						upload_enable;
			std::string					upload_path;
			size_t						client_max_body_size;
		};

		struct servers
		{
			std::vector<std::string>	names;
			std::string					host;
			std::string					root;
			std::map<int, std::string>	error_pages;
			std::vector<location>		locations;
			size_t						port;
		};
	
		std::string				file_;
		std::vector<servers>	servers_;

	public :
		Parsing();	
		Parsing(std::string file);
		~Parsing();

		void						parseConfig();
		void						parseServer();
		Parsing::servers			parseProps(std::string::iterator first, std::string::iterator end);
		Parsing::servers			returnProps(Parsing::servers server, std::vector<std::string> line);
		Parsing::location			parseLocation(std::string name, std::string::iterator first, std::string::iterator end);
		Parsing::location			returnLocation(Parsing::location location, std::vector<std::string> line);
		void						parseMethods();
		bool						parseName();
		bool						valid(std::string name, const char **valid_names);
		std::string					getNextLine(std::string::iterator *first, std::string::iterator end);
		void						skipWhite(std::string::iterator *first, std::string::iterator end);
		bool						compString(std::string::iterator *first, std::string::iterator end, std::string src);
		std::string::iterator		getBrackets(std::string::iterator next, std::string::iterator end);
		std::vector<std::string>	splitWhitespace(std::string str);
		int							to_int(char const *s, size_t count);
		std::vector<servers>		getServers() {	return (this->servers_); }
	
		class ParsingException : public std::exception { virtual const char *what() const throw(); };
};

#endif
