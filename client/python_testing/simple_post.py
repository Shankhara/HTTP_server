import requests as req
from printing_test import Printing_test as pt

class Test_post():

	def __init__(self, url_nginx_, url_webserv_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_

	def print_test_content(self, test_url_nginx, test_url_webserv, content_length):
		print("\tNginx url :	" + test_url_nginx)
		print("\tWebserv url : 	" + test_url_webserv + "\n")

		nginx = req.post(test_url_nginx)
		webserv = req.post(test_url_webserv)

		pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
		pt().test(webserv.headers['Content-Length'], content_length, "Checking header Content-Length")
		pt().test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

	def test00_post(self):
		print("\n\t\033[1;32mTest 00 -\033[0m POST / \n")

		test_url_nginx = self.url_nginx + "/";
		test_url_webserv = self.url_webserv + "/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	
	def test01_post(self):
		print("\n\t\033[1;32mTest 01 -\033[0m POST /a \n")
	
		test_url_nginx = self.url_nginx + "/a";
		test_url_webserv = self.url_webserv + "/a";
	
		self.print_test_content(test_url_nginx, test_url_webserv, "10")
	
	def test02_post(self):
		print("\n\t\033[1;32mTest 02 -\033[0m POST /a/ \n")
	
		test_url_nginx = self.url_nginx + "/a/";
		test_url_webserv = self.url_webserv + "/a/";
		
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
		
	def test03_post(self):
		print("\n\t\033[1;32mTest 03 -\033[0m POST /doesntexist \n")
	
		test_url_nginx = self.url_nginx + "/doesntexist";
		test_url_webserv = self.url_webserv + "/doesntexist";

		self.print_test_content(test_url_nginx, test_url_webserv, "156")
	
	def test04_post(self):
		print("\n\t\033[1;32mTest 04 -\033[0m POST /fake_dir \n")
	
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
	#		nginx = req.post(test_url_nginx)
	#	except ValueError:
	#		throw_nginx = 1;
	#	
	#	try:
	#		webserv = req.post(test_url_webserv)
	#	except ValueError:
	#		throw_webserv = 1;
	#
	#	test(str(throw_webserv), str(throw_nginx), "Requesting a directory as a file (should throw)")
	
	
	def test05_post(self):
		print("\n\t\033[1;32mTest 05 -\033[0m POST /fake_dir/ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/";
		test_url_webserv = self.url_webserv + "/fake_dir/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "255")
	
	def test06_post(self):
		print("\n\t\033[1;32mTest 06 -\033[0m POST /fake_dir/../ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../";
		test_url_webserv = self.url_webserv + "/fake_dir/../"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	
	def test07_post(self):
		print("\n\t\033[1;32mTest 07 -\033[0m POST /fake_dir/../. \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../.";
		test_url_webserv = self.url_webserv + "/fake_dir/../."; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "496")
	
	def test08_post(self):
		print("\n\t\033[1;32mTest 08 -\033[0m POST /fake_dir/../fake_dir/../../tmp/fake_dir \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../fake_dir/../../tmp/fake_dir";
		test_url_webserv = self.url_webserv + "/fake_dir/../fake_dir/../../tmp/fake_dir"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "156")
	
	def test09_post(self):
		print("\n\t\033[1;32mtest 09 -\033[0m POST /fake_dir/../fake_dir/../../ \n")
	
		test_url_nginx = self.url_nginx + "/fake_dir/../fake_dir/../../";
		test_url_webserv = self.url_webserv + "/fake_dir/../fake_dir/../../"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
	
	def test10_post(self):
		print("\n\t\033[1;32mtest 10 -\033[0m POST /../etc/passwd/ \n")
	
		test_url_nginx = self.url_nginx + "/../etc/passwd/";
		test_url_webserv = self.url_webserv + "/../etc/passwd/"; 
	
		self.print_test_content(test_url_nginx, test_url_webserv, "0")
	
	def test_simple_post(self):
		self.test00_post()
		self.test01_post()
		self.test02_post()
		self.test03_post()
		#test04_post()
		self.test05_post()
		self.test06_post()
		#test07_post()
		self.test08_post()
		self.test09_post()
		self.test10_post()	
