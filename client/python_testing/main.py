import os
import requests as req
from simple_get import Test_get as sg
from simple_post import Test_post as sp
from advanced_get import Test_advanced_get as ag
from simple_other_request import Test_other_request as other
from modifying_header import Test_modifying_header as mh
from printing_test import Printing_test as pt

PORT_NGINX = os.getenv('NGINX_PORT')
PORT_WEBSERV = os.getenv('WEBSERV_PORT')

URL_NGINX = "http://localhost:{}".format(PORT_NGINX);
URL_WEBSERV = "http://localhost:{}".format(PORT_WEBSERV);

if __name__ == "__main__":
	tests = [0,0]
	tests = sg(URL_NGINX, URL_WEBSERV, tests).test_simple_get()
#	tests = sp(URL_NGINX, URL_WEBSERV, tests).test_simple_post()
	tests = ag(URL_NGINX, URL_WEBSERV, tests).test_advanced_get()
	tests = other(URL_NGINX, URL_WEBSERV, tests).test_simple_other_request()
	tests = mh(URL_NGINX, URL_WEBSERV, tests).test_simple_other_request()

	if tests[0] != tests[1]:
		print("\n\n\t\t\033[1;31m" + str(tests[0]) + " / " + str(tests[1]) + "\tSuccessful Tests" + "\033[0m\n\n")
	else : 
		print("\n\n\t\t\033[1;32m" + str(tests[0]) + " / " + str(tests[1]) + "\tSuccessful Tests" + "\033[0m\n\n")
	
