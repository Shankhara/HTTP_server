#pragma once

#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <map>
#include "FileDescriptor.hpp"
#include "../Server.hpp"
#include "../Utils.hpp"
#include "../Log.hpp"

#define BUFFER_SIZE 32768
#define  MIME_FILE "/etc/mime.types2"

class Mime: public FileDescriptor
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
	void onEvent();
	void openFile_();
};
