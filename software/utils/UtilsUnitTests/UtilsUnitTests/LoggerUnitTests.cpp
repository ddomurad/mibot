#include "LoggerUnitTests.h"

using namespace mibot;
using namespace mibot::ut;

MockLoggerChannel::MockLoggerChannel():
    LoggerOutput(nullptr),
    WriteCallsCount(0)
{}

void MockLoggerChannel::Write(QString message)
{
    WriteCallsCount++;
    CallArguments.append(message);
}


LoggerUnitTests::LoggerUnitTests() :
    QObject(nullptr)
{

}

LoggerUnitTests::~LoggerUnitTests()
{

}

void LoggerUnitTests::testTestosteron()
{
    Assert.IsFale(false);
}

void LoggerUnitTests::testTestosteron2()
{
    Assert.AreNotEqual<double>(2.01,2.01);
}

