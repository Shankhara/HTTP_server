#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/time.h>

#define WEBSERV_ID "webserv/0.0.1"

std::string getStrDate();
bool isSpace(char);
std::string removeSpaces(std::string &);
char ft_tolower(char);
bool boolFind(std::string const & haystack, std::string const & needle);
std::string decode64(std::string const &str);
int ft_atoi(std::string const &str);
std::string ft_itoa(unsigned long);
size_t strHex_to_int(std::string const &str);
std::vector<std::string> explode(const std::string & str, const char & delim);
int getNextLine(std::string & buf, std::string & line);
unsigned long getTime();
char	*ft_strncpy(char *dest, const char *src, size_t n);

#endif
