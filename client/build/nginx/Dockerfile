FROM ubuntu:18.04
ARG  ROOT=/opt/webserv
RUN  mkdir -p ${ROOT}
RUN  apt-get update && apt-get upgrade -y
RUN  apt-get install nginx php-cli -y
ADD  virtualhost.conf /etc/nginx/sites-enabled/default
CMD ["nginx", "-g", "daemon off;"]
