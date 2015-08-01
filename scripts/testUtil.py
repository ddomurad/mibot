import sys, inspect

class UTest:
	def run(self):
		ms = inspect.getmembers(self, predicate=inspect.ismethod)
		self.initClass()

		for m in ms:
			if m[0].startswith("test"):
				self.initTest()
				try:
					print("Run test: %s" % m[0])
					m[1]()
					print("Success")
				except :
					print("Failure: %s" % e)
				self.cleanTest()
	
		self.cleanClass()

def execTests(name):
	cs = inspect.getmembers( sys.modules[name], inspect.isclass)
	for c in cs:
		mro = c[1].__mro__
		if mro[0].__name__ == "UTest":
			continue
		for b in mro:
			if b.__name__ == "UTest":
				inst = c[1]()
				inst.run()
				break

