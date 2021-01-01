import requests as req
from printing_test import Printing_test as pt

class Test_get():

	def __init__(self, url_nginx_, url_webserv_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_

	def print_test_content(self, test_url_nginx, test_url_webserv, content_length):
		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")

		nginx = req.get(test_url_nginx)
		webserv = req.get(test_url_webserv)

		pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
		pt().test(webserv.headers['Content-Length'], content_length, "Checking header Content-Length")
		pt().test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

	def test00_get(self):
		print("\n\t\033[1;32mTest 00 -\033[0m GET / \n")

		test_url_nginx = self.url_nginx + "/";
		test_url_webserv = self.url_webserv + "/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	
	def test01_get(self):
		print("\n\t\033[1;32mTest 01 -\033[0m GET /a \n")
	
		test_url_nginx = self.url_nginx + "/a";
		test_url_webserv = self.url_webserv + "/a";
	
		self.print_test_content(test_url_nginx, test_url_webserv, "10")
	
	def test02_get(self):
		print("\n\t\033[1;32mTest 02 -\033[0m GET /a/ \n")
	
		test_url_nginx = self.url_nginx + "/a/";
		test_url_webserv = self.url_webserv + "/a/";
		
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
		
	def test03_get(self):
		print("\n\t\033[1;32mTest 03 -\033[0m GET /doesntexist \n")
	
		test_url_nginx = self.url_nginx + "/doesntexist";
		test_url_webserv = self.url_webserv + "/doesntexist";

		self.print_test_content(test_url_nginx, test_url_webserv, "156")
	
	def test04_get(self):
		print("\n\t\033[1;32mTest 04 -\033[0m GET /fake_dir \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir";
		test_url_webserv = self.url_webserv + "/fake_dir"; 
	
		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")
	
		throw_nginx = 0;
		throw_webserv = 0;
	
	#
	#	Request is exiting the process is the connection is refused somehow
	#
	
	
	#	try:
	#		nginx = req.get(test_url_nginx)
	#	except ValueError:
	#		throw_nginx = 1;
	#	
	#	try:
	#		webserv = req.get(test_url_webserv)
	#	except ValueError:
	#		throw_webserv = 1;
	#
	#	test(str(throw_webserv), str(throw_nginx), "Requesting a directory as a file (should throw)")
	
	
	def test05_get(self):
		print("\n\t\033[1;32mTest 05 -\033[0m GET /fake_dir/ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/";
		test_url_webserv = self.url_webserv + "/fake_dir/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "255")
	
	def test06_get(self):
		print("\n\t\033[1;32mTest 06 -\033[0m GET /fake_dir/../ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../";
		test_url_webserv = self.url_webserv + "/fake_dir/../"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	
	def test07_get(self):
		print("\n\t\033[1;32mTest 07 -\033[0m GET /fake_dir/../. \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../.";
		test_url_webserv = self.url_webserv + "/fake_dir/../."; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	def test08_get(self):
		print("\n\t\033[1;32mTest 08 -\033[0m GET /fake_dir/../fake_dir/../../tmp/fake_dir \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../fake_dir/../../tmp/fake_dir";
		test_url_webserv = self.url_webserv + "/fake_dir/../fake_dir/../../tmp/fake_dir"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "156")
	
	def test09_get(self):
		print("\n\t\033[1;32mtest 09 -\033[0m GET /fake_dir/../fake_dir/../../ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../fake_dir/../../";
		test_url_webserv = self.url_webserv + "/fake_dir/../fake_dir/../../"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
	
	def test10_get(self):
		print("\n\t\033[1;32mtest 10 -\033[0m GET /../etc/passwd/ \n")
	
		test_url_nginx = self.url_nginx + "/../etc/passwd/";
		test_url_webserv = self.url_webserv + "/../etc/passwd/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
	
	def test_simple_get(self):
		self.test00_get()
		self.test01_get()
		self.test02_get()
		self.test03_get()
		#test04_get()
		self.test05_get()
		self.test06_get()
		#test07_get()
		self.test08_get()
		self.test09_get()
		self.test10_get()	
