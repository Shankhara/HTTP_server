import requests as req
from printing_test import Printing_test as pt

class Test_get():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_content(self, path, content_length, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m GET " + path + " \n")
	
		test_url_nginx = self.url_nginx + path;
		test_url_webserv = self.url_webserv + path;

		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")

		nginx = req.get(test_url_nginx)
		webserv = req.get(test_url_webserv)

		self.tests = pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code", self.tests)
		self.tests = pt().test(webserv.headers['Content-Length'], content_length, "Checking header Content-Length", self.tests)
		self.tests = pt().test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type", self.tests)

	def print_test_throw(self, path, test_name, test_number):	

		print("\n\t\033[1;32mTest " + test_number + " -\033[0m GET " + path + " \n")
	
		test_url_nginx = self.url_nginx + path;
		test_url_webserv = self.url_webserv + path;

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
		self.print_test_content("/", "618", "00")
	
	def test01_get(self):
		self.print_test_content("/a", "10", "01")
	
	def test02_get(self):
		self.print_test_content("/a/", "0", "02")
		
	def test03_get(self):
		self.print_test_content("/doesntexist", "156", "03")
	
	def test04_get(self):
		self.print_test_throw("/fake_dir", "Requesting a directory as a file (should throw)", "04")	
	
	def test05_get(self):
		self.print_test_content("/fake_dir/", "255", "05")
	
	def test06_get(self):
		self.print_test_content("/fake_dir/../", "618", "06")
	
	def test07_get(self):
		self.print_test_throw("/fake_dir/../.", "Requesting . (should throw)", "07")	
	
	def test08_get(self):
		self.print_test_content("/fake_dir/../fake_dir/../../tmp/fake_dir", "156", "08")
	
	def test09_get(self):
		self.print_test_content("/fake_dir/../fake_dir/../../", "0", "09")
	
	def test10_get(self):
		self.print_test_content("/../etc/passwd/", "0", "10")
	
	def test_simple_get(self):
		self.test00_get()
		self.test01_get()
		self.test02_get()
		self.test03_get()
		self.test04_get()
		self.test05_get()
		self.test06_get()
		self.test07_get()
		self.test08_get()
		self.test09_get()
		self.test10_get()
		return self.tests	
