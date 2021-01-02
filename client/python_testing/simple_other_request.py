import requests as req
from printing_test import Printing_test as pt

class Test_other_request():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_content(self, test_url_nginx, test_url_webserv, content_length):
		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")

		nginx = req.get(test_url_nginx)
		webserv = req.get(test_url_webserv)

		self.tests = pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code", self.tests)
		self.tests = pt().test(webserv.headers['Content-Length'], content_length, "Checking header Content-Length", self.tests)
		self.tests = pt().test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type", self.tests)

	def print_test_throw(self, test_url_nginx, test_url_webserv, test_name):	
		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")
	
		throw_nginx = False;
		throw_webserv = False;	
		try:
			nginx = req.get(test_url_nginx)
		except req.exceptions.RequestException as e:
			throw_nginx = True;
	
		try:
			webserv = req.get(test_url_webserv)
		except req.exceptions.RequestException as e:
			throw_webserv = True;
		self.tests = pt().test(str(throw_webserv), str(throw_nginx), test_name, self.tests)
	
	def test00_get(self):
		print("\n\t\033[1;32mTest 00 -\033[0m GET / \n")

		test_url_nginx = self.url_nginx + "/";
		test_url_webserv = self.url_webserv + "/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "618")
	
	def test_simple_other_request(self):
		self.test00_get()
		return self.tests	
