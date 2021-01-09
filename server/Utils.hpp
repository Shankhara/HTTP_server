#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <cstring>
#include <stdlib.h>
#include <vector>
#include <string>
#include <algorithm>
#include <sys/time.h>

#define WEBSERV_ID "webserv/0.0.2"

int ft_atoi(std::string const &str);
int getNextLine(std::string & buf, std::string & line);
unsigned long getTime();
char ft_tolower(char);
char ft_toupper(char);
char *ft_strcat(char *s1, char *s2);
char *ft_strncpy(char *dest, const char *src, size_t n);
size_t ft_strlen(char *str);
bool ft_strcmp(char *s1, char *s2);
bool isSpace(char);
bool boolFind(std::string const & haystack, std::string const & needle);
std::string removeSpaces(std::string &);
std::string decode64(std::string const &str);
std::string getStrDate();
std::string ft_itoa(unsigned long);
size_t strHexToInt(std::string str);
std::vector<std::string> explode(const std::string &, const char &);
std::vector<std::string> explode(const std::string &, const char &, size_t);

#endif
