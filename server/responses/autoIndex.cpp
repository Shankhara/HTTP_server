#include "RespGet.hpp"
#include "../Utils.hpp"
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>

std::string RespGet::doAutoIndexTemplate_() {
	DIR				*dir = opendir("/tmp");
	struct dirent 	*ent;
	struct stat		t_stat;
	size_t			size;
	std::string		space;
	std::string		tmp;
	std::string 	body = 	"<html>"
							"<head><title>Index of /</title></head>"
							"<body bgcolor=\"white\">"
							"<h1>Index of /</h1><hr><pre><a href=\"../\">../</a>\n";

	while ((ent = readdir(dir)) != NULL)
	{
		stat(stds(stds("/tmp/") + stds(ent->d_name)).c_str(), &t_stat);
		tmp = stds(ent->d_name);
		if (tmp[0] == '.')
			continue;
		size = tmp.size();
		if (tmp.size() >= 50)
		{
			tmp = tmp.substr(0, 47);
			tmp += "..&gt";
			size = 50;
		}
		space.resize(51 - size, ' ');	
    	body += "<a href=\"";
		body += stds(ent->d_name);
		body += "\">";
		body += tmp;
		body += "</a>";
		body += space;
		tmp = stds(asctime(localtime(&t_stat.st_ctime)));
		tmp = tmp.substr(0, tmp.size() - 1);
		body += tmp;
		space.resize(19, ' ');	
		body += space;
		if (ent->d_type == DT_DIR)
			body += "-";
		else
			body += ft_itoa(t_stat.st_size);	
		body += "\n";
 	}
	body += "</pre><hr></body>";
	body += "</html>";
	return body;
}

/*
<html>
<head><title>Index of /</title></head>
<body bgcolor="white">
<h1>Index of /</h1><hr><pre><a href="../">../</a>
<a href="Temp-d3af0dbe-3f22-495d-adc4-e666de5cac8d/">Temp-d3af0dbe-3f22-495d-adc4-e666de5cac8d/</a>         26-Dec-2020 01:55                   -
<a href="Temp-e99f8774-09d9-47fc-9e6b-c53d8ac33e41/">Temp-e99f8774-09d9-47fc-9e6b-c53d8ac33e41/</a>         26-Dec-2020 01:55                   -
<a href="ssh-dAgD7f2yfPMG/">ssh-dAgD7f2yfPMG/</a>                                  24-Dec-2020 15:11                   -
<a href="systemd-private-7c467a78216d4af7810e75e30a2b1629-ModemManager.service-EZLhww/">systemd-private-7c467a78216d4af7810e75e30a2b162..&gt;</a> 24-Dec-2020 15:11                   -
<a href="systemd-private-7c467a78216d4af7810e75e30a2b1629-redis-server.service-jsiSus/">systemd-private-7c467a78216d4af7810e75e30a2b162..&gt;</a> 24-Dec-2020 15:11                   -
<a href="systemd-private-7c467a78216d4af7810e75e30a2b1629-rtkit-daemon.service-Wg3ZsS/">systemd-private-7c467a78216d4af7810e75e30a2b162..&gt;</a> 24-Dec-2020 15:11                   -
<a href="systemd-private-7c467a78216d4af7810e75e30a2b1629-systemd-resolved.service-gCFEdk/">systemd-private-7c467a78216d4af7810e75e30a2b162..&gt;</a> 24-Dec-2020 15:11                   -
<a href="config-err-VNP2ld">config-err-VNP2ld</a>                                  24-Dec-2020 15:11                   0
</pre><hr></body>
</html>
*/

int RespGet::writeAutoIndex_() {
	std::string body = doAutoIndexTemplate_();
	appendHeaders("text/html", body.size());
	append_(body); //TODO: tterrail if body.size > BUFFER_SIZE content-Length will not match body sent;
	return (nbytes_);
}
