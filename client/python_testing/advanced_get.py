import requests as req
#import grequests as greq
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

	def test00_get(self):
		self.print_test_value("/", "GET / (Multiple Requests Synchronously)", "00")
		for i in range(10):
			webserv = req.get(self.url_webserv + "/")
			self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
			self.tests = pt().test(str(webserv.headers["Content-Length"]), str(len(webserv.text)), "Checking content-length", self.tests)
	
	def test01_get(self):
		reqs = []
		self.print_test_value("/", "GET / (Multiple Requests Asynchronously)", "01")
		for i in range(10):
			reqs.append(greq.get(self.url_webserv + "/"))
		responses = greq.map(reqs, size=50)
		for webserv in responses:
			self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
			self.tests = pt().test(str(webserv.headers["Content-Length"]), str(len(webserv.text)), "Checking content-length", self.tests)

	def test02_get(self):
		self.print_test_value("/", "GET / (Huge payload)", "02")
		for i in range(10):
			rand = random.randint(1000, 100000000)
			data = "r" * rand
			webserv = req.get(self.url_webserv + "/", data=data)
			self.tests = pt().test(str(webserv.status_code), "200", "Body size : " + str(rand), self.tests)
	
	def test_advanced_get(self):
		self.test00_get()
#		self.test01_get()
		self.test02_get()
		return self.tests	
