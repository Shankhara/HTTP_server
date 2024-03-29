/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: racohen <racohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/08 16:17:34 by racohen           #+#    #+#             */
/*   Updated: 2021/01/19 15:47:33 by racohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

# include <string>
# include <iostream>
# include <list>
# include <map>
# include <unistd.h>
# include <fcntl.h>
# include "Ut.hpp"
# include <iostream>
# include <cstdlib>
# include <vector>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# define	DEFAULT_PATH "./parsing/test/default.conf"
# define	UNUSED(x)	((void)(x))

static const char 		*serverProps_[] = { "listen",
											"server_name",
											"error_page",
											"root",
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
												"client_max_body_size",
												"auth_basic",
												"auth_basic_user_file",
												0 };

static int	line_;
static int	char_;

class Parsing
{
	public :
		typedef std::string					stds;
		typedef stds::iterator				iterator;
		typedef std::pair<int, stds>        pi;
		typedef std::pair<stds, stds>       ps;

	struct location
	{
		stds					name;
		stds					root;
		std::vector<stds>		methods;
		bool					autoindex;
		stds					index;
		std::vector<stds>		cgi_extension;
		stds					cgi_path;
		stds					auth_basic;
		stds					auth_basic_user_file;
		bool					upload_enable;
		stds					upload_path;
		size_t					client_max_body_size;
	};

	struct server
	{
		std::vector<stds>		names;
		stds					host;
		stds					root;
		std::vector<pi>			error_pages;
		std::vector<location>	locations;
		unsigned int			port;
		size_t					client_max_body_size;
	};


private :
	stds					file_;
	std::vector<server>		servers_;
	static Parsing          *instance_;
	stds					content_;

private:
	Parsing();
	Parsing(stds file);

public :
		~Parsing();

		static 						Parsing *getInstance();
		void 						setFile(const stds &file);
		void						parseConfig();
		void						parseServer();
		Parsing::server				parseProps(iterator first, iterator end);
		Parsing::server				returnProps(Parsing::server server, std::vector<stds> line, std::vector<int> *prop);
		Parsing::location			parseLocation(stds name, iterator first, iterator end);
		Parsing::location			returnLocation(Parsing::location location, std::vector<stds> line, std::vector<int> *prop);
		Parsing::server				getDefaultServer();
		Parsing::location			getDefaultLocation();
		std::vector<int>			getTableDef();
		bool           	    		compString(iterator *first, iterator end, stds src);
		bool                		parseSemi(stds *src);
		stds						getNextLine(iterator *first, iterator end);
		void						skipWhite(iterator *first, iterator end, bool inc);
		std::vector<server>			getServers() {	return (this->servers_); }
		void setContent(const stds &content);

	class ParsingException : public std::exception
	{
		private:
			std::string		msg_;

		public:
			ParsingException(std::string file, std::string msg = "Configfile error.")
			{
				//if (serverProps_ == methods_ && locationProps_ == methods_)
				//	UNUSED(li);	
				this->msg_ = std::string(file + std::string(":") + this->to_string(line_) + std::string(":") + this->to_string(char_) + std::string(": error: ") + std::string(msg));
			}

			std::string         to_string(int convert)
			{
   				std::string res;

			    while (convert > 9)
   				{
			        res += char(convert % 10 + 48);
   			     	convert /= 10;
    			}
   				res += char(convert % 10 + 48);
    			return (res);
			}
			~ParsingException() throw() {};
			const char *what () const throw () { return (msg_.c_str()); }
	};	
};

#endif
