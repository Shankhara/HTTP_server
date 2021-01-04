import requests as req
from printing_test import Printing_test as pt

class Test_other_request():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, method, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m  GET " + path + " \n")	
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")

	def test00_get(self):
		self.print_test_value("/", "GET", "00")
		headers = {'content-type' : 'application/json'}
		webserv = req.get(self.url_webserv + "/")

	def test_simple_other_request(self):
		self.test00_get()
		return self.tests	
