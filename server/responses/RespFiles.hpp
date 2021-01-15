#ifndef WEBSERV_RESPFILES_HPP
#define WEBSERV_RESPFILES_HPP

#include <vector>
#include "RespFile.hpp"
#include "../Request.hpp"

class RespFiles: public RespFile {
public:
	RespFiles(const Request &r, char buf[], unsigned int bufSize);
	virtual ~RespFiles();

protected:
	bool contentLangNegotiated_;
	void negotiateContentLang_();
	void writeFiles_();
	void openFiles_(int, int);
    void reachResource_();

private:
	std::vector<int> fds_;

};

#endif //WEBSERV_RESPFILES_HPP
