import requests as req
from printing_test import Printing_test as pt

class Test_other_request():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, method, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m  " + method + " \n")	
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")

	def test00_trace(self):
		self.print_test_value("/", "TRACE", "00")
		webserv = req.request('TRACE', self.url_webserv + "/")
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)

	def test01_head(self):
		self.print_test_value("/", "HEAD", "01")	
		webserv = req.head(self.url_webserv + "/")
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		self.tests = pt().test(webserv.text, "", "Checking for empty content", self.tests)

	def test02_put(self):
		self.print_test_value("/post/test", "PUT", "02")
		webserv = req.put(self.url_webserv + "/post/test", data="put content")	
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		webserv = req.get(self.url_webserv + "/post/test")
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		self.tests = pt().test(webserv.text, "put content", "Checking content of get after put request", self.tests)
	
	def test03_delete(self):
		self.print_test_value("/post/test", "DELETE", "03")
		test_url_webserv = self.url_webserv + "/post/test";	
		
		res = req.get(test_url_webserv)
		self.tests = pt().test(str(res.status_code), "200", "Checking status code of get /post/test", self.tests)
		self.tests = pt().test(res.text, "delete and put content", "Checking content of get /post/test", self.tests)
		webserv = req.delete(test_url_webserv)	
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code of get /post/test", self.tests)	
		res = req.get(test_url_webserv)
		self.tests = pt().test(str(res.status_code), "404", "Checking status code of get after deleting file", self.tests)

	def test_simple_other_request(self):
	#	self.test00_trace()
		self.test01_head()
		self.test02_put()
		self.test03_delete()
		return self.tests	
