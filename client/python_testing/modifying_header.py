import requests as req
import random
from printing_test import Printing_test as pt

class Test_modifying_header():

	def __init__(self, url_nginx_, url_webserv_, tests_):
		self.url_nginx = url_nginx_
		self.url_webserv = url_webserv_
		self.tests = tests_

	def print_test_value(self, path, method, test_number):
		print("\n\t\033[1;32mTest " + test_number + " -\033[0m  GET " + path + " \n")	
		print("\tWebserv url : 	" + self.url_webserv + path + "\n")


	def test_request_get_headers(self, headers):
		print(headers)
		webserv = req.get(self.url_webserv + "/", headers=headers)
		self.tests = pt().test(str(webserv.status_code), "200", "Checking status code", self.tests)
		self.tests = pt().test(str(webserv.headers["Content-Length"]), str(len(webserv.text)), "Checking content-length", self.tests)

	def test_request_get_headers_length(self, headers, length):
		print("Current Header Length : " + str(length))
		webserv = req.get(self.url_webserv + "/", headers=headers)
		nginx = req.get(self.url_nginx + "/", headers=headers)
		self.tests = pt().test(str(webserv.status_code), str(nginx.status_code), "Checking status code", self.tests)
	
	def test00_accept_charsets(self):
		self.print_test_value("/", "GET (accept_charsets) ", "00")
		headers = {'accept-charset' : 'iso-8859-1'}
		self.test_request_get_headers(headers)	
		headers = {'accept-charset' : 'utf-8, iso-8859-1;q=0.5'}
		self.test_request_get_headers(headers)	
		headers = {'accept-charset' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		headers = {'ACCEPt-charset' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		headers = {'ACCEPt-charset ' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		headers = {'ACCEPt-charsett' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		headers = {'ACCEPt-c' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		for i in range(50):
			size = random.randint(5000, 10000)
			first = "a" * size
			headers = {'accept-charset' : first}
			self.test_request_get_headers_length(headers, size)
			headers = {first : "iso-8859-1"}
			self.test_request_get_headers_length(headers, size)

	def test01_accept_language(self):
		self.print_test_value("/", "GET (accept_language) ", "01")

	def test02_allow(self):
		self.print_test_value("/", "GET (allow) ", "02")

	def test03_authorization(self):
		self.print_test_value("/", "GET (authorization) ", "03")

	def test04_content_language(self):
		self.print_test_value("/", "GET (content_language) ", "04")

	def test05_content_length(self):
		self.print_test_value("/", "GET (content_length) ", "05")

	def test06_content_location(self):
		self.print_test_value("/", "GET (content_location) ", "06")

	def test07_content_type(self):
		self.print_test_value("/", "GET (content_type) ", "07")

	def test08_date(self):
		self.print_test_value("/", "GET (date) ", "08")

	def test09_host(self):
		self.print_test_value("/", "GET (host) ", "09")

	def test10_last_modified(self):
		self.print_test_value("/", "GET (last_modified) ", "10")

	def test11_location(self):
		self.print_test_value("/", "GET (location) ", "11")

	def test12_referer(self):
		self.print_test_value("/", "GET (referer) ", "12")

	def test13_transfer_encoding(self):
		self.print_test_value("/", "GET (transfer_encoding) ", "13")

	def test14_user_agent(self):
		self.print_test_value("/", "GET (user_agent) ", "14")

	def test_simple_other_request(self):
		self.test00_accept_charsets();		
		self.test01_accept_language();		
		self.test02_allow();		
		self.test03_authorization();		
		self.test04_content_language();		
		self.test05_content_length();		
		self.test06_content_location();		
		self.test07_content_type();		
		self.test08_date();		
		self.test09_host();		
		self.test10_last_modified();		
		self.test11_location();		
		self.test12_referer();		
		self.test13_transfer_encoding();		
		self.test14_user_agent();		

		return self.tests	
