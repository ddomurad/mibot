
#include "LoggerUnitTests.h"
#include "NetworkUnitTests.h"

using namespace mibot;
using namespace mibot::ut;

int main()
{
    TEST_ADD(LoggerUnitTests);
    TEST_ADD(NetworkUnitTests);

    auto res = TEST_FLUSH();
    TEST_RES_PRINT(res);
    TEST_RES_FILE(res,"results");
}
