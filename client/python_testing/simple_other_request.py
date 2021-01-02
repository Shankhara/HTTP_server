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
	
	def test00_trace(self):
		print("\n\t\033[1;32mTest 00 -\033[0m TRACE / \n")

		test_url_webserv = self.url_webserv + "/";	

		print("\tWebserv url : 	" + test_url_webserv + "\n")

		webserv = req.request('TRACE', test_url_webserv)

		print(webserv.text)
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)

	def test01_head(self):
		print("\n\t\033[1;32mTest 01 -\033[0m HEAD / \n")

		test_url_webserv = self.url_webserv + "/";	

		print("\tWebserv url : 	" + test_url_webserv + "\n")

		webserv = req.head(test_url_webserv)

		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		self.tests = pt().test(webserv.text, "", "Checking for empty content", self.tests)

	def test02_put(self):
		print("\n\t\033[1;32mTest 02 -\033[0m PUT /post/test \n")

		test_url_webserv = self.url_webserv + "/post/test";	

		print("\tWebserv url : 	" + test_url_webserv + "\n")

		webserv = req.put(test_url_webserv, data="put content")
	
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		
		webserv = req.get(test_url_webserv)
		
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		self.tests = pt().test(webserv.text, "put content", "Checking content of get after put request", self.tests)
			

	def test03_delete(self):
		print("\n\t\033[1;32mTest 03 -\033[0m DELETE /post/test \n")

		test_url_webserv = self.url_webserv + "/post/test";	

		print("\tWebserv url : 	" + test_url_webserv + "\n")

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
