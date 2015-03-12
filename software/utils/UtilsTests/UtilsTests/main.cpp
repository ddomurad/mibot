#include <QCoreApplication>
#include <cstdio>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibUnitTest.h>

#include "TestHeader.h"

int main()
{   
    TEST_ADD(MyTest);
    mibot::TestResult res = TEST_FLUSH();
    TEST_RES_FILE(res, "./test_res");
    TEST_RES_PRINT(res);
}
