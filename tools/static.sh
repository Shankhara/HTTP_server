echo "Boostrapping static files"
WEBSERV="/tmp/webserv"
DIR=${WEBSERV}"/static"

rm -Rf ${WEBSERV}/static
mkdir -p ${DIR}/dir ${DIR}/errors

echo "<html><head><title>Static</title></head>
<body><h3>It works</h3>
<ul>
<li><a href=\"./dir\">dir</a></li>
<li><a href=\"./dir/\">dir</a></li>
</ul>
<body><html>" > ${DIR}/page.html

echo "<html><head><title>404</title></head>
<body><h3>Absolutely not found</h3>
<body><html>" > ${DIR}/errors/404.html

ln -s ${DIR}/page.html ${DIR}/index.html

echo "server {
	listen 127.0.0.1:8080;
	server_name localhost;
	error_page 404 ${DIR}/errors/404.html;
	root /tmp;
	location / {
		index index.html;
		root ${DIR};
	}
	location /autoindex {
		autoindex on;
	}
	location /auth {
		auth_basic on;
		autoindex on;
	}
}" > ${WEBSERV}/static.conf
echo "Building Webserv"

cd ../server && make > /dev/null && cd ../tools
../server/webserv ${WEBSERV}/static.conf


