#FROM ubuntu:18.04
#ARG  ROOT=/opt/webserv
#RUN  mkdir -p ${ROOT}
#RUN  apt-get update && apt-get upgrade -y
#RUN  apt-get install php-cli -y
FROM ft_ubuntu
ADD  webserv /usr/local/bin
COPY  default.conf /etc/nginx/sites-enabled/default.conf
CMD ["/usr/local/bin/webserv", "/etc/nginx/sites-enabled/default.conf"]
