FROM ubuntu:18.04
ARG  ROOT=/opt/webserv
RUN  mkdir -p ${ROOT}
RUN  apt-get update && apt-get upgrade -y
RUN  apt-get install php-cli -y
ADD  webserv /usr/local/bin
ADD  virtualhost.conf /etc/nginx/sites-enabled/default
CMD ["/usr/local/bin/webserv", "/etc/nginx/sites-enabled/default"]
