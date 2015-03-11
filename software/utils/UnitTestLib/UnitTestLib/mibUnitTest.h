#ifndef UNITTEST_H
#define UNITTEST_H

#include "mibUnitTestlib_global.h"

#include <QtCore>
#include <QObject>

namespace mibot
{

class UNITTESTLIBSHARED_EXPORT UnitTest
{
public:
    UnitTest() {}
    virtual ~UnitTest(){}

    virtual void ClassInit() {}
    virtual void TestInit() {}
    virtual void TestClean() {}
    virtual void ClassClean() {}
};

class UNITTESTLIBSHARED_EXPORT UnitTestManager
{
public:
    static QList<QPair<QString,QString>> Flush();
    static void RegTest(QString, UnitTest *);

private:
    static QList<QPair<QString, UnitTest*>> _utests;

    static QList<QPair<QString,QString>> ExecuteTest(QString, UnitTest*);
};

class _Assert
{
public:
    _Assert(int line);

    template <typename T>
    void AreEqual(T expeced, T actual)
    {
        if(expeced != actual)
            ThrowError(QString("Assert::AreEqual failure. Expected: <%1>, Actual: <%2>").arg(expeced).arg(actual));
    }

    template <typename T>
    void AreNotEqual(T notExpeced, T actual)
    {
        if(notExpeced == actual)
            ThrowError(QString("Assert::AreNotEqual failure. NotExpeced: <%1>, Actual: <%2>").arg(notExpeced).arg(actual));
    }

    void IsTrue(bool state)
    {
        if(state)
            ThrowError(QString("Assert::IsTrue failure."));
    }

    void IsFale(bool state)
    {
        if(state)
            ThrowError(QString("Assert::IsFale failure."));
    }

    template <typename T>
    void IsPtr(T * ptr)
    {
        if(ptr == nullptr)
            ThrowError(QString("Assert::IsPtr failure."));
    }

    template <typename T>
    void IsNullPtr(T * ptr)
    {
        if(ptr != nullptr)
            ThrowError(QString("Assert::IsNullPtr failure."));
    }

    void Message(QString message)
    {
        ThrowError(message);
    }

private:
    int _line;
    void ThrowError(QString message);
};

#define Assert _Assert(__LINE__)
}
#endif // UNITTEST_H
