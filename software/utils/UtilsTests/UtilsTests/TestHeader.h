#ifndef TESTHEADER_H
#define TESTHEADER_H

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibUnitTest.h>

class MyTest : public QObject, public mibot::UnitTest
{
    Q_OBJECT
public:

    MyTest() : QObject(0)
    {}

    ~MyTest(){}

    void ClassInit(void *)
    {
        qDebug() << "class init";
    }
    void TestInit()
    {
        qDebug() << "test init";
    }
    void TestClean()
    {
        qDebug() << "test clean";
    }
    void ClassClean()
    {
        qDebug() << "class clean";
    }

public slots:
    void successTest()
    {
        qDebug() << "test 1";
    }

    void errorTest()
    {
        qDebug() << "test 2";
        mibot::Assert.Message("test");
    }

    void errorTest2()
    {
        mibot::Assert.AreEqual<int>(2,3);
    }
};


#endif // TESTHEADER_H

