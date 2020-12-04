#pragma once

#include <vector>
#include "utils.hpp"

class request
{
	private:
	std::vector<std::string> requestLine_;

	public:
	request();
	~request();
	
	read();
	parse();
};
