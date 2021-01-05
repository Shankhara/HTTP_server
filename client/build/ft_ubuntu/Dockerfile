FROM ubuntu:18.04
ARG  ROOT=/opt/webserv
RUN  mkdir -p ${ROOT}
RUN  mkdir /cores
RUN  apt-get update && apt-get upgrade -y
RUN  apt-get install curl php-cli -y
ADD  ./. /tmp/
