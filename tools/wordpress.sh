#!/bin/bash
WEBSERV="/tmp/webserv"
DIR=${WEBSERV}"/wordpress"

echo "Bootstrapping in ${WEBSERV}"

mkdir -p ${DIR}/nop ${DIR}/Yeah ${DIR}/put_test/
wget -q -P ${WEBSERV} -nc https://fr.wordpress.org/latest-fr_FR.tar.gz 
sudo apt install php-cgi php-mysql mariadb-server
echo "Untar in ${DIR}"
tar xzf ${WEBSERV}/latest-fr_FR.tar.gz -C ${WEBSERV} 

echo "server {
	listen 127.0.0.1:8080;
	server_name localhost;
	location / {
		cgi_extension .php;
		cgi_path /usr/bin/php-cgi;
		index index.php index.html;
		root ${DIR};
	}
}" > ${WEBSERV}/wordpress.conf

cd ${PWD}
echo "Building webserv"
cd ../server && make > /dev/null && cd ../tools
../server/webserv ${WEBSERV}/wordpress.conf
