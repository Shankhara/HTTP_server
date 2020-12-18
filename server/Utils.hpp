#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <cstring>
#include <sstream>
#include <vector>
#include <string>

char ft_tolower(char);
bool boolFind(std::string const & haystack, std::string const & needle);
std::string decode64(std::string const &str);
int ft_atoi(std::string const &str);
size_t strHex_to_int(std::string const &str);
std::vector<std::string> explode(const std::string & str, const char & delim);
int getNextLine(std::string & buf, std::string & line);

#endif
