/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 16:17:34 by racohen           #+#    #+#             */
/*   Updated: 2020/12/15 15:25:12 by racohen          ###   ########.fr       */
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
# include "Ut.hpp"
# include <iostream>
# include <cstdlib>
# include <vector>
# include <sstream>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# define	DEFAULT_PATH "./parsing/test/nginx.conf"
# define	UNUSED(x)	((void)(x))

static const char 		*serverProps_[] = { "listen",
											"server_name",
											"error_page",
											"root",
											"access_log",
											"client_max_body_size",
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
		typedef std::string					stds;
		typedef stds::iterator					iterator;
		typedef std::pair<int, stds>           pi;
		typedef std::pair<stds, stds>          ps;

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
		std::vector<pi>			error_pages;
		std::vector<location>	locations;
		std::vector<ps>			access_log;
		size_t					port;
		size_t					client_max_body_size;
	};


private :
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
		Parsing::servers			getDefaultServer();
		Parsing::location			getDefaultLocation();
		bool                		compString(iterator *first, iterator end, stds src);
		bool                		parseSemi(stds *src);
		stds						getNextLine(iterator *first, iterator end);
		void						skipWhite(iterator *first, iterator end, bool inc);
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
				//if (serverProps_ == methods_ && locationProps_ == methods_)
				//	UNUSED(li);
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
