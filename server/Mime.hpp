#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>

#include "Utils.hpp"

#define BUFFER_SIZE 32768

class Mime {

	private:
	std::vector<std::string> mime_;
	std::vector<std::vector<std::string> > extensions_;

	std::string getExtension(std::string &);

	public:
	Mime();
	~Mime();

	void parseMimeFile();
	std::string getContentType(std::string &) const;
	std::string getExtension(std::string & filename) const;
};
