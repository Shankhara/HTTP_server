#!/bin/bash

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

docker run -it --rm -d -p 8082:8082 --name ft_ngiserv ft_nginx
docker run -it --rm -d -p 8081:8082 --name ft_webserv ft_webserv

