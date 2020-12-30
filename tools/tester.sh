#!/bin/bash
WEBSERV="/tmp/webserv"
DIR=${WEBSERV}"/YoupiBanane"

echo "boostraping in ${WEBSERV}"

rm -rf ${DIR}/put_test/
mkdir -p ${DIR}/nop ${DIR}/Yeah ${DIR}/put_test/
wget -P ${WEBSERV} -nc https://projects.intra.42.fr/uploads/document/document/2467/ubuntu_tester
wget -P ${WEBSERV} -nc https://projects.intra.42.fr/uploads/document/document/2465/ubuntu_cgi_tester
chmod +x ${WEBSERV}/ubuntu_*
echo "<h1>index</h1>" > ${DIR}/youpi.bad_extension
touch ${DIR}/youpi.bla
touch ${DIR}/nop/youpi.bad_extension
touch ${DIR}/nop/other.pouic
touch ${DIR}/Yeah/not_happy.bad_extension

echo "server {
	listen 127.0.0.1:8080;
	server_name localhost;
	root ${DIR};
	location / {
		method GET;
		index youpi.bad_extension;
	}
	location /put_test/ {
		method PUT;
	}
	location /post_body {
		method POST;
		client_max_body_size 100;
		root ${DIR};
		index youpi.bad_extension;
	}
	location /directory {
		method GET POST;
		cgi_extension .bla;
		cgi_path ${WEBSERV}/ubuntu_cgi_tester;
		root ${DIR};
		index youpi.bad_extension;
	}
}" > ${WEBSERV}/servers.conf

cd ${PWD}
cd ../server && make && cd ../tools
../server/webserv ${WEBSERV}/servers.conf
