echo "Boostrapping static files"
WEBSERV="/tmp/webserv"
DIR=${WEBSERV}"/static"

rm -Rf ${WEBSERV}/static
mkdir -p ${DIR}/dir

echo "<html><head><title>Static</title></head>
<body><h3>It works</h3>
<ul>
<li><a href=\"./dir\">dir</a></li>
<li><a href=\"./dir/\">dir</a></li>
</ul>
<body><html>" > ${DIR}/index.html


echo "server {
	listen 127.0.0.1:8080;
	server_name localhost;
	location / {
		index index.html;
		root ${DIR};
	}
}" > ${WEBSERV}/static.conf
echo "Building Webserv"

cd ../server && make > /dev/null && cd ../tools
../server/webserv ${WEBSERV}/static.conf


