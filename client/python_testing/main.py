import os
import requests as req

PORT_NGINX = os.getenv('NGINX_PORT')
PORT_WEBSERV = os.getenv('WEBSERV_PORT')

URL_NGINX = "http://localhost:{}".format(PORT_NGINX);
URL_WEBSERV = "http://localhost:{}".format(PORT_WEBSERV);


def success(test_name):
	print("\033[1;32mSuccess\033[0m: > " + test_name)

def failed(test_name):
	print("\033[1;31mFail\033[0m: > " + test_name)


def test(value, expected, test_name):
	if value != expected:
		failed(test_name)
		print(" Value : " + value)
		print(" Expected : " + expected)
	else:
		success(test_name)

def print_test_content(test_url_nginx, test_url_webserv, content_length):
	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)

	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Length'], content_length, "Checking header Content-Length")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")


def test00_get():
	print("\n\t\033[1;32mTest 00 -\033[0m GET / \n")

	test_url_nginx = URL_NGINX + "/";
	test_url_webserv = URL_WEBSERV + "/"; 

	print_test_content(test_url_nginx, test_url_webserv, "496")


def test01_get():
	print("\n\t\033[1;32mTest 01 -\033[0m GET /a \n")

	test_url_nginx = URL_NGINX + "/a";
	test_url_webserv = URL_WEBSERV + "/a";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)

	test(webserv.headers['Content-Length'], nginx.headers['Content-Length'], "Checking header Content-Length")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test02_get():
	print("\n\t\033[1;32mTest 02 -\033[0m GET /a/ \n")

	test_url_nginx = URL_NGINX + "/a/";
	test_url_webserv = URL_WEBSERV + "/a/";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)

	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")
	
def test03_get():
	print("\n\t\033[1;32mTest 03 -\033[0m GET /doesntexist \n")

	test_url_nginx = URL_NGINX + "/doesntexist";
	test_url_webserv = URL_WEBSERV + "/doesntexist";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)
	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test04_get():
	print("\n\t\033[1;32mTest 04 -\033[0m GET /fake_dir \n")

	test_url_nginx = URL_NGINX + "/fake_dir";
	test_url_webserv = URL_WEBSERV + "/fake_dir"; 

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


def test05_get():
	print("\n\t\033[1;32mTest 05 -\033[0m GET /fake_dir/ \n")

	test_url_nginx = URL_NGINX + "/fake_dir/";
	test_url_webserv = URL_WEBSERV + "/fake_dir/"; 

	print_test_content(test_url_nginx, test_url_webserv, "255")

def test06_get():
	print("\n\t\033[1;32mTest 06 -\033[0m GET /fake_dir/../ \n")

	test_url_nginx = URL_NGINX + "/fake_dir/../";
	test_url_webserv = URL_WEBSERV + "/fake_dir/../"; 

	print_test_content(test_url_nginx, test_url_webserv, "496")


def test07_get():
	print("\n\t\033[1;32mTest 07 -\033[0m GET /fake_dir/../. \n")

	test_url_nginx = URL_NGINX + "/fake_dir/../.";
	test_url_webserv = URL_WEBSERV + "/fake_dir/../."; 

	print_test_content(test_url_nginx, test_url_webserv, "496")

def test08_get():
	print("\n\t\033[1;32mTest 08 -\033[0m GET /fake_dir/../fake_dir/../../tmp/fake_dir \n")

	test_url_nginx = URL_NGINX + "/fake_dir/../fake_dir/../../tmp/fake_dir";
	test_url_webserv = URL_WEBSERV + "/fake_dir/../fake_dir/../../tmp/fake_dir"; 

	print_test_content(test_url_nginx, test_url_webserv, "156")

def test09_get():
	print("\n\t\033[1;32mtest 09 -\033[0m GET /fake_dir/../fake_dir/../../ \n")

	test_url_nginx = URL_NGINX + "/fake_dir/../fake_dir/../../";
	test_url_webserv = URL_WEBSERV + "/fake_dir/../fake_dir/../../"; 

	print_test_content(test_url_nginx, test_url_webserv, "0")

def test10_get():
	print("\n\t\033[1;32mtest 10 -\033[0m GET /../etc/passwd/ \n")

	test_url_nginx = URL_NGINX + "/../etc/passwd/";
	test_url_webserv = URL_WEBSERV + "/../etc/passwd/"; 

	print_test_content(test_url_nginx, test_url_webserv, "0")

def test_simple_get():
	test00_get()
	test01_get()
	test02_get()
	test03_get()
	#test04_get()
	test05_get()
	test06_get()
	#test07_get()
	test08_get()
	test09_get()
	test10_get()

def test00_post():
	print("\n\t\033[1;32mTest 00 -\033[0m POST / \n")
   
	test_url_nginx = URL_NGINX + "/"
	test_url_webserv = URL_WEBSERV + "/"

	print("\tNginx url :    " + test_url_nginx)
	print("\tWebserv url :  " + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)	

	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Length'], "496", "Checking header Content-Length")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test01_post():
	print("\n\t\033[1;32mTest 01 -\033[0m POST /a \n")

	test_url_nginx = URL_NGINX + "/a";
	test_url_webserv = URL_WEBSERV + "/a";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)

	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Length'], nginx.headers['Content-Length'], "Checking header Content-Length")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test02_post():
	print("\n\t\033[1;32mTest 02 -\033[0m POST /a/ \n")

	test_url_nginx = URL_NGINX + "/a/";
	test_url_webserv = URL_WEBSERV + "/a/";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)

	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test03_post():
	print("\n\t\033[1;32mTest 03 -\033[0m POST /doesntexist \n")

	test_url_nginx = URL_NGINX + "/doesntexist";
	test_url_webserv = URL_WEBSERV + "/doesntexist";

	print("\tNginx url :	" + test_url_nginx)
	print("\tWebserv url : 	" + test_url_webserv + "\n")

	nginx = req.get(test_url_nginx)
	webserv = req.get(test_url_webserv)
	test(str(webserv.status_code), str(nginx.status_code), "Checking status code")
	test(webserv.headers['Content-Type'], nginx.headers['Content-Type'], "Checking header Content-Type")

def test_simple_post():
	test00_post()
	test01_post()
	test02_post()
	test03_post()

if __name__ == "__main__":
	test_simple_get()
#	test_simple_post()
