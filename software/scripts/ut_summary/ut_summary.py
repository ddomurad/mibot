import cfg
import os
from os import listdir

def PrepareTestEnv(TEST):
	print("Preparing test <%s>" % TEST[1]);
	os.system("rm %s" % TEST[0]+TEST[2])

def RunTest(TEST):
	print("Running test <%s>" % TEST[1]);
	currentDir = os.getcwd()
	os.chdir(TEST[0])
	os.system("./%s" % TEST[1])
	os.chdir(currentDir);

def GetTestResults(TEST):
	print("Copy results from test <%s>" % TEST[1]);
	os.system("cp %s %s" % (TEST[0]+TEST[2], cfg.OUT_DIR + TEST[2] ))

def ExecuteAllTests():
	print("Cleaning output directory ")
	os.system("rm %s*" % cfg.OUT_DIR)
	for test in cfg.TESTS:
		print("-------- = %s = --------" % test[1] )
		PrepareTestEnv(test)
		RunTest(test)
		GetTestResults(test)
		print("\n");

def GetCharsIndices(line, c):
	rtab = []
	for i in range(len(line)):
		if line[i] == c:
			rtab.append(i)
	return rtab
			
def GetTestsResults():
	return listdir(cfg.OUT_DIR)

def ParseTestEntry(testEntry):
	testEntry = testEntry.strip()
	indices = GetCharsIndices(testEntry,':')
	
	if(len(indices) < 3):
		raise Exception("(len(indices) < 3")

	test_desc = testEntry[0:indices[2]];
	test_desc_split = test_desc.split("::");
	test_result = testEntry[indices[2] +2: len(testEntry)];
	isSuccess = test_result == "SUCCESS";
	
	return [test_desc_split[0], test_desc_split[1], test_result, isSuccess];

def FormatEntry(module, entry):
	
	#return "<div class=\"success\"><table class=\"res_table_g\"> <tr><td class=\"test_class\">testClass1</td><td class=\"test_name\">test1</td><td class=\"test_result\">result</td></tr> </table></div>"
	return "<div class=\"%s\"><table class=\"%s\"> <tr><td class=\"test_class\">%s</td><td class=\"test_name\">%s</td><td class=\"test_result\">%s</td></tr> </table></div>" \
	% ( "success" if entry[3] else "error" ,"res_table_g" if entry[3] else "res_table_r",entry[0],entry[1],entry[2])

def ParseTestResult(testResultFile, fo):
	f = open(cfg.OUT_DIR + testResultFile, "r")
	while True:
		line = f.readline()
		if(line == ""):
			break

		entry = ParseTestEntry(line)
		fo.write(FormatEntry(testResultFile,entry));

	f.close();

def ReadFiletoEnd(fname):
	f = open(fname,"r")
	s = f.read();
	f.close()
	return s

def GenerateHtmlDocument():
	fo = open(cfg.OUT_DIR + "index.html", "w")
	fo.write("<html><head><style>");
	fo.write(ReadFiletoEnd("./css.css"))
	fo.write("</style>");
	fo.write("<title>UnitTestsRaport</title>")
	fo.write("<script>")
	fo.write(ReadFiletoEnd("./js.js"))
	fo.write("</script>")
	fo.write("</head><body><input type=\"text\" value=\"\" onchange=\"onFilter(this.value)\"/> ")
	

	for tr in GetTestsResults():
		ParseTestResult (tr, fo)

	fo.close()


print("### .:: EXECUTING TESTS ::. ###\n")
ExecuteAllTests()

print("### .:: GENERATING HTML ::. ###\n")
GenerateHtmlDocument()
