#include "RespGet.hpp"

std::string	RespGet::returnLineTemplate_(stds body, std::list<stds>::iterator first, struct stat)
{
	size_t						size;
	std::string					space;
	std::string					tmp;
	
	tmp = first->substr(0, first->size() - 1);
	size = tmp.size();
	if (tmp.size() >= 50)
	{
		tmp = tmp.substr(0, 47);
		tmp += "..&gt";
		size = 50;
	}
	space.resize(51 - size, ' ');	
    body += "<a href=\"";
	body += first->substr(0, first->size() - 1);
	if ((*first)[first->size() - 1] == DT_DIR)
		body += "/";
	body += "\">";
	body += tmp;
	body += "</a>";
	body += space;
	tmp = "-";
	tmp = tmp.substr(0, tmp.size() - 1);
	body += tmp;
	space.resize(19, ' ');	
	body += space;
	return (body);
}

std::string RespGet::doAutoIndexTemplate_(stds path) {
	DIR							*dir = opendir(path.c_str());
	struct dirent 				*ent;
	struct stat					t_stat;
	std::list<stds>				sorted;
	std::list<stds>				file;
	std::list<stds>::iterator	first;
	stds						tmp;
	stds		 				body = 	"<html>"
										"<head><title>Index of /</title></head>"
										"<body bgcolor=\"white\">"
										"<h1>Index of /</h1><hr><pre><a href=\"../\">../</a>\n";

	if (dir == NULL)
		return ("");
	while ((ent = readdir(dir)) != NULL)
		if ((ent->d_name)[0] != '.')
		{
			tmp = stds(ent->d_name);
			tmp += ent->d_type;
			sorted.push_back(tmp);
		}
	sorted.sort();
	first = sorted.begin();
	while (first != sorted.end())
	{
		if ((*first)[first->size() - 1] == DT_DIR)
		{
			stat(stds(path + stds("/") + first->substr(0, first->size() - 1)).c_str(), &t_stat);
			body = returnLineTemplate_(body, first, t_stat);
			body += "-";
			body += "\n";
			first++;
 		}
		else
		{
			file.push_back(*first);
			first++;
		}	
	}
	first = file.begin();
	while (first != file.end())
	{
		stat(stds(path + stds("/") + first->substr(0, first->size() - 1)).c_str(), &t_stat);
		body = returnLineTemplate_(body, first, t_stat);
		body += ft_itoa(t_stat.st_size);
		body += "\n";
		first++;
	}	
	body += "</pre><hr></body>";
	body += "</html>";
	closedir(dir);
	return body;
}

int RespGet::writeAutoIndex_(stds path) {
	if (autoIndexBody_.empty() && !headersBuilt_) {
		autoIndexBody_ = doAutoIndexTemplate_(path);
		appendHeaders(200, "text/html", autoIndexBody_.size());
	}
	if (static_cast<unsigned int>(nbytes_) + 1 == bufSize_)
		return (nbytes_);
	int s = bufSize_ - 1 - nbytes_;
	if (autoIndexBody_.size() + nbytes_ >= bufSize_)
	{
		append_(autoIndexBody_.substr(0, s));
		autoIndexBody_.assign(autoIndexBody_, s, autoIndexBody_.size() - s);
	}
	else if (!autoIndexBody_.empty())
	{
		append_(autoIndexBody_);
		autoIndexBody_.clear();
	}
	return (nbytes_);
}


