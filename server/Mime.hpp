#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>

#include "Utils.hpp"

#define BUFFER_SIZE 32768

class Mime
{
	private:
	std::map<std::string, std::string> mimeTypes_;
	static Mime * instance_;

	public:
	Mime();
	~Mime();
	void parseMimeFile();
	std::string getContentType(std::string &);
	std::string getExtension(const std::string &);
	std::string getFilename(std::string &) const;
	static Mime * getInstance();
};
