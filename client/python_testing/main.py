import os
import requests
import unittest

PORT_NGINX = os.getenv('NGINX_PORT')
PORT_WEBSERV = os.getenv('WEBSERV_PORT')

URL_NGINX = "http://localhost:{}/".format(PORT_NGINX);
URL_WEBSERV = "http://localhost:{}/".format(PORT_WEBSERV);

class Test01(unittest.TestCase):
	def test_root(self):
		r = requests.get(URL_WEBSERV)
		self.assertEqual(r.status_code, 200)

if __name__ == "__main__":
	unittest.main()	
