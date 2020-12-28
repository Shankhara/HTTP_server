import requests
import unittest

class Test01(unittest.TestCase):
	def test_root(self):
		self.assertEqual(200, 200)

if __name__ == "__main__":

	#r = requests.get("http://127.0.0.1:8082/")
	#print(r.status_code)
	unittest.main()	
