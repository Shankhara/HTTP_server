class Printing_test():
	def success(self, test_name):
		print("\033[1;32mSuccess\033[0m: > " + test_name)

	def failed(self, test_name):
		print("\033[1;31mFail\033[0m: > " + test_name)

	def test(self, value, expected, test_name):
		if value != expected:
			self.failed(test_name)
			print(" Value : " + value)
			print(" Expected : " + expected)
		else:
			self.success(test_name);
