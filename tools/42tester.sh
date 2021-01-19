#!/bin/bash
WEBSERV="/tmp/webserv"
DIR=${WEBSERV}"/YoupiBanane"

echo "Bootstrapping in ${WEBSERV}"

rm -rf ${WEBSERV}/YoupiBanane/
mkdir -p ${DIR}/nop ${DIR}/Yeah ${DIR}/put_test_of_your_choice/
wget -q -P ${WEBSERV} -nc https://projects.intra.42.fr/uploads/document/document/2467/ubuntu_tester 
wget -q -P ${WEBSERV} -nc https://projects.intra.42.fr/uploads/document/document/2465/ubuntu_cgi_tester
chmod +x ${WEBSERV}/ubuntu_*
echo "<h1>index</h1>" > ${DIR}/youpi.bad_extension
touch ${DIR}/youpi.bla
touch ${DIR}/nop/youpi.bad_extension
touch ${DIR}/nop/other.pouic
touch ${DIR}/Yeah/not_happy.bad_extension

echo "server {
	listen 127.0.0.1:8080;
	server_name localhost;
	root ${DIR}/;
	location / {
		method GET;
		index youpi.bad_extension;
	}
	location /put_test/ {
		method PUT;
		root ${DIR}/put_test_of_your_choice/;
	}
	location /post_body {
		method POST;
		client_max_body_size 100;
	}
	location /directory/ {
		method GET POST;
		cgi_extension .bla;
		cgi_path ${WEBSERV}/ubuntu_cgi_tester;
		root ${DIR}/;
		index youpi.bad_extension;
	}
}" > ${WEBSERV}/servers.conf

cd ${PWD}
echo "Building webserv"
cd ../server && make > /dev/null && cd ../tools
../server/webserv ${WEBSERV}/servers.conf &
${WEBSERV}/ubuntu_tester http://localhost:8080
pkill -SIGINT webserv
