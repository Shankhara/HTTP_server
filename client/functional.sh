#!/bin/bash

#cp build/virtualhost.conf build/webserv
#cp build/virtualhost.conf build/nginx

cd ../server/ && make
cp webserv ../client/build/webserv
cd ../client/build/ft_ubuntu
docker build -t ft_ubuntu .
cd ../nginx
#docker build -t ft_nginx .
cd ../webserv
docker build -t ft_webserv .

docker stop ft_ngi > /dev/null 2>&1
docker stop ft_web > /dev/null 2>&1

#docker run -it --rm -d -p 8082:8082 --name ft_ngi ft_nginx
#docker run -it --rm -d -p 8082:8082 --name ft_web ft_webserv
#docker run -it --rm -d -p 8082:8082 --name ft_ngi ft_nginx
docker run -it --rm -d -p 8081:8081 --name ft_web ft_webserv

