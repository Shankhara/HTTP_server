import requests as req
import grequests as greq
import random

from printing_test import Printing_test as pt

class Test_advanced_get():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, test_name, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m  " + test_name + " \n")
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")

	def check_header_status_code(self, status_code, content_length):
		if status_code != "200":
			self.tests = pt().test(status_code, "200",\
				"Testing multiple requests Status Code", self.tests)
			return False
		elif content_length != "618":
			self.tests = pt().test(content_length, "618",\
				"Testing multiple requests Content-Length", self.tests)	
			return False
		return True	
	
	def test00_get(self):
		self.print_test_value("/", "GET / (Multiple Requests Synchronously)", "00")
		for i in range(200):
			webserv = req.get(self.url_webserv + "/")
			if self.check_header_status_code(str(webserv.status_code),\
				 str(webserv.headers["Content-Length"])) == False:
				return
		self.tests = pt().test("1", "1", "Testing multiple requests synchronously", self.tests)	
	
	def test01_get(self):
		reqs = []
		self.print_test_value("/", "GET / (Multiple Requests Asynchronously)", "00")
		for i in range(50):
			reqs.append(greq.get(self.url_webserv + "/"))
		responses = greq.map(reqs, size=50)
		for webserv in responses:
			if self.check_header_status_code(str(webserv.status_code),\
				 str(webserv.headers["Content-Length"])) == False:
				return		
		self.tests = pt().test("1", "1", "Testing multiple requests asynchronously", self.tests)

	def test02_get(self):
		self.print_test_value("/", "GET / (Huge payload)", "02")
		for i in range(10):
			rand = random.randint(1000, 10000000)
			data = "r" * rand
			webserv = req.get(self.url_webserv + "/", data=data)
			self.tests = pt().test(str(webserv.status_code), "200", "Header size : " + str(rand), self.tests)
	
	def test_advanced_get(self):
		self.test00_get()
		#self.test01_get()
		self.test02_get()
		return self.tests	
