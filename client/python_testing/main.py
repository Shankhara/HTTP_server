import os
import requests as req
from simple_get import Test_get as sg
from simple_post import Test_post as sp
from printing_test import Printing_test as pt

PORT_NGINX = os.getenv('NGINX_PORT')
PORT_WEBSERV = os.getenv('WEBSERV_PORT')

URL_NGINX = "http://localhost:{}".format(PORT_NGINX);
URL_WEBSERV = "http://localhost:{}".format(PORT_WEBSERV);

if __name__ == "__main__":
	sg(URL_NGINX, URL_WEBSERV).test_simple_get()
	sp(URL_NGINX, URL_WEBSERV).test_simple_post()
