#!/bin/bash

cp build/virtualhost.conf build/webserv
cp build/virtualhost.conf build/nginx

cd ../server/ && make re
cp webserv ../client/build/webserv
cd ../client/build/nginx
docker build -t ft_nginx .
cd ../webserv
docker build -t ft_webserv .

docker stop ft_nginx > /dev/null 2>&1
docker stop ft_webserv > /dev/null 2>&1

docker run -d --rm --name ft_nginx -p 9090:8080 ft_nginx
docker run -d --rm --name ft_webserv -p 9091:8080 ft_webserv
