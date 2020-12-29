#!/bin/bash

export NGINX_PORT=9082
export WEBSERV_PORT=9081

cd ../server/ && make
cp webserv ../client/build/webserv
cd ../client/build/ft_ubuntu
docker build -t ft_ubuntu .
cd ../nginx
docker build -t ft_nginx .
cd ../webserv
docker build -t ft_webserv .

docker stop ft_nginx > /dev/null 2>&1
docker stop ft_webserv > /dev/null 2>&1

docker run --rm -d -p $NGINX_PORT:8082 --name ft_nginx ft_nginx
docker run --rm -d -p $WEBSERV_PORT:8082 --name ft_webserv ft_webserv
