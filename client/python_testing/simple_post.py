import requests as req
from printing_test import Printing_test as pt

class Test_post():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m POST " + path + " \n")	
		print("\tNginx url :	" + self.url_nginx + path)
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")
	

	def print_test_content(self, path, test_number):
		self.print_test_value(path, test_number)	
		for i in range (100):
			nginx = req.post(self.url_nginx + path)
			webserv = req.post(self.url_webserv + path)
			self.tests = pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code", self.tests)
			#self.tests = pt().test(webserv.headers['Content-Length'], str(len(webserv.text)), "Checking header Content-Length", self.tests)
			self.tests = pt().test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type", self.tests)

	def print_test_throw(self, path, test_name, test_number):	
		self.print_test_value(path, test_number)	
		for i in range (100):
			throw_webserv = False;	
			throw_nginx = False;
			try:
				nginx = req.post(self.url_nginx + path)
			except req.exceptions.RequestException as e:
				throw_nginx = True;	
			try:
				webserv = req.post(self.url_webserv + path)
			except req.exceptions.RequestException as e:
				throw_webserv = True;
			self.tests = pt().test(str(throw_webserv), str(throw_nginx), test_name, self.tests)

	def test00_post(self):
		self.print_test_content("/", "00")
	
	def test01_post(self):
		self.print_test_content("/a", "01")
	
	def test02_post(self):
		self.print_test_content("/a/", "02")
		
	def test03_post(self):
		self.print_test_content("/doesntexist", "03")
	
	def test04_post(self):
		self.print_test_throw("/fake_dir", "Requesting a directory as a file (should throw)", "04")	

	def test05_post(self):
		self.print_test_content("/fake_dir/", "05")
	
	def test06_post(self):
		self.print_test_content("/fake_dir/../", "06")
	
	def test07_post(self):
		self.print_test_throw("/fake_dir/../.", "Requesting . (should throw)", "07")	
	
	def test08_post(self):
		self.print_test_content("/fake_dir/../fake_dir/../../tmp/fake_dir", "08")
	
	def test09_post(self):
		self.print_test_content("/fake_dir/../fake_dir/../../", "09")
	
	def test10_post(self):
		self.print_test_content("/../etc/passwd/", "10")
	
	def test_simple_post(self):
		self.test00_post()
		self.test01_post()
		self.test02_post()
		self.test03_post()
		self.test04_post()
		self.test05_post()
		self.test06_post()
		self.test07_post()
		self.test08_post()
		self.test09_post()
		self.test10_post()
		return self.tests	
