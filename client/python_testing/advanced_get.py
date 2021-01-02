import requests as req
import grequests as greq

from printing_test import Printing_test as pt

class Test_advanced_get():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def check_header_status_code(self, status_code, content_length):
		if status_code != "200":
			self.tests = pt().test(status_code, "200", "Testing multiple requests Status Code", self.tests)
			return False
		elif content_length != "618":
			self.tests = pt().test(content_length, "618", "Testing multiple requests Content-Length", self.tests)	
			return False
		return True	
	
	def test00_get(self):
		print("\n\t\033[1;32mTest 00 -\033[0m GET / (Multiple Requests Synchronously)\n")

		test_url_webserv = self.url_webserv + "/"; 
	
		for i in range(200):
			webserv = req.get(test_url_webserv)
			if self.check_header_status_code(str(webserv.status_code), str(webserv.headers["Content-Length"])) == False:
				return
		self.tests = pt().test("1", "1", "Testing multiple requests synchronously", self.tests)	
	

	def test01_get(self):
		print("\n\t\033[1;32mTest 01 -\033[0m GET / (Multiple Requests Asynchronously)\n")

		test_url_webserv = self.url_webserv + "/";
	
		reqs = []
		for i in range(50):
			reqs.append(greq.get(test_url_webserv))
		responses = greq.map(reqs, size=50)
		for webserv in responses:
			if self.check_header_status_code(str(webserv.status_code), str(webserv.headers["Content-Length"])) == False:
				return		
		print("slaut")
		self.tests = pt().test("1", "1", "Testing multiple requests asynchronously", self.tests)	
	

	def test_advanced_get(self):
		self.test00_get()
		#self.test01_get()
		return self.tests	
