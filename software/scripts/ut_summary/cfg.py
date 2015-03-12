
ROOT_DIR = "../../"
TESTS = []

def AddTestCfg(testList,testFolder, testExecName, testResultFileName):
	return testList.append([ROOT_DIR + testFolder, testExecName, testResultFileName])




AddTestCfg(TESTS,"utils/UtilsTests/debug/","UtilsTests","test_res")
AddTestCfg(TESTS,"utils/UtilsUnitTests/debug/","UtilsUnitTests","results")

OUT_DIR = "./out/"
