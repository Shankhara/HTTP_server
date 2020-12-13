/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 16:17:34 by racohen           #+#    #+#             */
/*   Updated: 2020/12/13 06:24:16 by racohen          ###   ########.fr       */
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

static int	line_;
static int	char_;

class Parsing
{
	public :
		typedef typename std::string			stds;
		typedef typename stds::iterator			iterator;

	private :

		struct location
		{
			stds					name;
			stds					root;
			std::vector<stds>		methods;
			bool					autoindex;
			stds					index;
			std::vector<stds>		cgi_extension;
			stds					cgi_path;
			bool					upload_enable;
			stds					upload_path;
			size_t					client_max_body_size;
		};

		struct servers
		{
			std::vector<stds>		names;
			stds					host;
			stds					root;
			std::map<int, stds>		error_pages;
			std::vector<location>	locations;
			std::map<stds, stds>	access_log;
			size_t					port;
		};
	
		stds					file_;
		std::vector<servers>	servers_;

	public :
		Parsing();	
		Parsing(stds file);
		~Parsing();

		void						parseConfig();
		void						parseServer();
		Parsing::servers			parseProps(iterator first, iterator end);
		Parsing::servers			returnProps(Parsing::servers server, std::vector<stds> line);
		Parsing::location			parseLocation(stds name, iterator first, iterator end);
		Parsing::location			returnLocation(Parsing::location location, std::vector<stds> line);
		bool						parseName();
		Parsing::servers			getDefaultServer();
		Parsing::location			getDefaultLocation();
		size_t             			getMcbs(stds s);
		bool                		parseSemi(stds *src);
		bool						valid(stds name, const char **valid_names);
		stds						getNextLine(iterator *first, iterator end);
		void						skipWhite(iterator *first, iterator end, bool inc);
		bool						compString(iterator *first, iterator end, stds src);
		iterator					getBrackets(iterator next, iterator end);
		std::vector<stds>			splitWhitespace(stds str);
		stds						getStatic(char const *s);
		int							to_int(char const *s, size_t count);
		std::vector<servers>		getServers() {	return (this->servers_); }

	class ParsingException : public std::exception
	{
		private:
			std::string		msg_;

		public:
			ParsingException(std::string file, std::string msg = "Configfile error.")
			{
				std::string	li;	
				std::string	ch;

				std::ostringstream conv;
				std::ostringstream conv2;
				conv << line_;
				li = std::string(conv.str());	
				conv2 << char_;
				ch = std::string(conv2.str());	
				this->msg_ = std::string(file + std::string(":") + li + std::string(":") + ch + std::string(": error: ") + std::string(msg));
			}		
			~ParsingException() throw() {};
			const char *what () const throw () { return (msg_.c_str()); }
	};	
};

#endif
