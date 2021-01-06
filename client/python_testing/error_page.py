import requests as req
import grequests as greq
import random

from printing_test import Printing_test as pt

class Test_error_page():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, test_name, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m  " + test_name + " \n")
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")

	def test00_get(self):
		self.print_test_value("/", "GET / (Multiple Requests Synchronously)", "00")
	
	def test_error_page(self):
		self.test00_get()
		self.test01_get()
		self.test02_get()
		return self.tests	
