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
		headers = {'ACCEPt-charsett' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		headers = {'ACCEPt-c' : 'utf-8, iso-8859-1;q=0.5, *;q=0.1'}
		self.test_request_get_headers(headers)
		'''for i in range(50):
			size = random.randint(5000, 10000)
			first = "a" * size
			headers = {'accept-charset' : first}
			self.test_request_get_headers_length(headers, size)
			headers = {first : "iso-8859-1"}
			self.test_request_get_headers_length(headers, size)
		'''
	def test01_accept_language(self):
		self.print_test_value("/", "GET (accept_language) ", "01")
		headers = {'accept-language' : 'de'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'de-CH'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'en-US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'en-US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'fjaskd;'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'De'}
		self.test_request_get_headers(headers)
		headers = {'accept-language' : 'DE'}
		self.test_request_get_headers(headers)
	
	def test02_allow(self):
		self.print_test_value("/", "GET (allow) ", "02")
		headers = {'allow' : 'GET, POST, HEAD'}
		self.test_request_get_headers(headers)
		headers = {'allow' : 'CONNECT, POST, HEAD'}
		self.test_request_get_headers(headers)
		headers = {'allow' : 'CONnECT, POST, HEAD'}
		self.test_request_get_headers(headers)
		headers = {'allow' : 'CONNECT, POST, HEAD, jfalsd;j'}
		self.test_request_get_headers(headers)	

	def test03_authorization(self):
		self.print_test_value("/", "GET (authorization) ", "03")
		headers = {'authorization' : 'Basic dXNlcjQyOnVzZXI0Mg=='}
		self.test_request_get_headers(headers)	
		headers = {'authorization' : 'BASIC dXNlcjQyOnVzZXI0Mg=='}
		self.test_request_get_headers(headers)	
		headers = {'authorization' : 'BASI dXNlcjQyOnVzZXI0Mg=='}
		self.test_request_get_headers(headers)	
		headers = {'authorization' : 'Basicc dXNlcjQyOnVzZXI0Mg=='}
		self.test_request_get_headers(headers)	
		headers = {'authorization' : 'Basic dXNlcjQyOnVzZXI0Mg='}
		self.test_request_get_headers(headers)	
	
	def test04_content_language(self):
		self.print_test_value("/", "GET (content_language) ", "04")
		headers = {'content-language' : 'de'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'de-CH'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'en-US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'en-US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'US,en;q=0.5'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'fjaskd;'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'De'}
		self.test_request_get_headers(headers)
		headers = {'content-language' : 'DE'}
		self.test_request_get_headers(headers)

	def test05_content_length(self):
		self.print_test_value("/", "GET (content_length) ", "05")
		headers = {'content-length' : '10'}
	#	self.test_request_get_headers(headers)
		headers = {'content-length' : '0'}
		self.test_request_get_headers(headers)
		headers = {'content-length' : '-0'}
		self.test_request_get_headers(headers)
		headers = {'content-length' : '-1'}
		self.test_request_get_headers(headers)
		headers = {'content-length' : '-2147483648'}
		self.test_request_get_headers(headers)
		headers = {'content-length' : '2147483647'}
		self.test_request_get_headers(headers)
		headers = {'content-length' : '2147483648'}
		self.test_request_get_headers(headers)

	def test06_content_location(self):
		self.print_test_value("/", "GET (content_location) ", "06")
		headers = {'content-location' : '/tmp'}
		self.test_request_get_headers(headers)
		headers = {'content-location' : 'localhost'}
		self.test_request_get_headers(headers)
		headers = {'content-location' : '/127.0.0.1'}
		self.test_request_get_headers(headers)
		
	def test07_content_type(self):
		self.print_test_value("/", "GET (content_type) ", "07")
		headers = {'content-type' : 'multipart/form-data; boundary=---------------------------974767299852498929531610575'}
		self.test_request_get_headers(headers)
		headers = {'content-type' : 'text/html'}
		self.test_request_get_headers(headers)
		headers = {'content-type' : 'text/'}
		self.test_request_get_headers(headers)
		headers = {'content-type' : 'text/htm'}
		self.test_request_get_headers(headers)
		headers = {'content-type' : 'text/HTML'}
		self.test_request_get_headers(headers)
	
	def test08_date(self):
		self.print_test_value("/", "GET (date) ", "08")
		headers = {'date' : 'Wed, 21 Oct 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'date' : 'WED, 21 Oct 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'date' : 'WED, 21 Oct 2015'}	
		self.test_request_get_headers(headers)
		headers = {'date' : 'WED, 21 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'date' : 'MON, 1 1969 00:01:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'date' : 'MON, 1 2147483847 00:01:00 GMT'}
		self.test_request_get_headers(headers)

	def test09_host(self):
		self.print_test_value("/", "GET (host) ", "09")
		headers = {'host' : 'localhost:9082'}
		self.test_request_get_headers(headers)
		headers = {'host' : 'localhost:9081'}
		self.test_request_get_headers(headers)
		headers = {'host' : '127.0.0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'host' : '127.0.0.1:2147483647'}
		self.test_request_get_headers(headers)
		headers = {'host' : '127.0.-0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'host' : '257.0.0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'host' : 'google.com:80'}
		self.test_request_get_headers(headers)
		headers = {'host' : 'google.com:'}
		self.test_request_get_headers(headers)

	def test10_last_modified(self):
		self.print_test_value("/", "GET (last_modified) ", "10")
		headers = {'last_modified' : 'Wed, 21 Oct 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'last_modified' : 'WED, 21 Oct 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'last_modified' : 'WED, 21 Oct 2015'}	
		self.test_request_get_headers(headers)
		headers = {'last_modified' : 'WED, 21 2015 07:28:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'last_modified' : 'MON, 1 1969 00:01:00 GMT'}
		self.test_request_get_headers(headers)
		headers = {'last_modified' : 'MON, 1 2147483847 00:01:00 GMT'}
		self.test_request_get_headers(headers)

	def test11_location(self):
		self.print_test_value("/", "GET (location) ", "11")
		headers = {'location' : '/'}
		self.test_request_get_headers(headers)
		headers = {'location' : '/index.html'}
		self.test_request_get_headers(headers)
		headers = {'location' : '/a'}
		self.test_request_get_headers(headers)
		headers = {'location' : '-------'}
		self.test_request_get_headers(headers)
		headers = {'location' : '/.//././/./'}
		self.test_request_get_headers(headers)
		headers = {'location' : '/../etc/passwd'}
		self.test_request_get_headers(headers)

	def test12_referer(self):
		self.print_test_value("/", "GET (referer) ", "12")
		headers = {'referer' : 'http://localhost:9082'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://localhost:9081'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'https://127.0.0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://127.0.0.1:2147483647'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://127.0.-0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://257.0.0.1:9081'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://google.com:80'}
		self.test_request_get_headers(headers)
		headers = {'referer' : 'http://google.com'}
		self.test_request_get_headers(headers)

	def test13_transfer_encoding(self):
		self.print_test_value("/", "GET (transfer_encoding) ", "13")
		headers = {'transfer-encoding' : 'chunked'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'compress'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'deflate'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'gzip'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'identity'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'identity, gzip,'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'identity, gzip'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'identity, GZIp'}
		self.test_request_get_headers(headers)
		headers = {'transfer-encoding' : 'identity, p'}
		self.test_request_get_headers(headers)
		
	def test14_user_agent(self):
		self.print_test_value("/", "GET (user_agent) ", "14")
		headers = {'user-agent' : 'Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:47.0) Gecko/20100101 Firefox/47.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla/5.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla/-1.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla/1.0 (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla/1.0                           (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla///1.0                           (Macintosh; Intel Mac OS X x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		headers = {'user-agent' : 'Mozilla///1.0   (Macintosh; Intel Mac OS Y x.y; rv:42.0) Gecko/20100101 Firefox/42.0'}
		self.test_request_get_headers(headers)
		
	def test_simple_other_request(self):
		self.test00_accept_charsets();		
		self.test01_accept_language();		
		self.test02_allow();		
	#	self.test03_authorization();		
		self.test04_content_language();		
#		self.test05_content_length();		
		self.test06_content_location();		
		self.test07_content_type();		
		self.test08_date();		
		self.test09_host();		
		self.test10_last_modified()
		self.test11_location();		
		self.test12_referer();		
		#self.test13_transfer_encoding();		
		self.test14_user_agent();		

		return self.tests	
