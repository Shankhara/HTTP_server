FROM ft_ubuntu
RUN  apt-get install nginx -y
COPY  ./default /etc/nginx/sites-enabled/default
CMD ["nginx", "-g", "daemon off;"]
