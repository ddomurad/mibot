#ifndef UNITTEST_H
#define UNITTEST_H

#include "mibUnitTestlib_global.h"

#include <QtCore>
#include <QObject>

namespace mibot
{

typedef QList<QPair<QString,QString>> TestResult;

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

    static void ResultsPtr(QList<QPair<QString,QString>>);
    static void ResultsToTextFile(QString fileName, QList<QPair<QString,QString>>);

private:
    static QList<QPair<QString, UnitTest*>> _utests;
    static QList<QPair<QString,QString>> ExecuteTest(QString, UnitTest*);
};

class _Assert
{
public:
    _Assert(int line);
    _Assert(int line, QString str);

    template <typename T>
    void AreEqual(T expeced, T actual, QString exp = QString())
    {
        if(expeced != actual)
            ThrowError(QString("Assert::AreEqual failure. Expected: [%1], Actual: [%2]").arg(expeced).arg(actual),exp);
    }

    template <typename T>
    void AreNotEqual(T notExpeced, T actual, QString exp = QString())
    {
        if(notExpeced == actual)
            ThrowError(QString("Assert::AreNotEqual failure. NotExpeced: [%1], Actual: [%2]").arg(notExpeced).arg(actual),exp);
    }

    template <typename T>
    void AreEqualNP(T expeced, T actual, QString exp = QString())
    {
        if(expeced != actual)
            ThrowError(QString("Assert::AreEqualNP failure."),exp);
    }

    template <typename T>
    void AreNotEqualNP(T notExpeced, T actual, QString exp = QString())
    {
        if(notExpeced == actual)
            ThrowError(QString("Assert::AreNotEqualNP failure."),exp);
    }

    template <typename T>
    void ArePtrEqual(T *expeced, T *actual, QString exp = QString())
    {
        if(expeced != actual)
            ThrowError(QString("Assert::ArePtrEqual failure."),exp);
    }

    template <typename T>
    void ArePtrNotEqual(T *notExpeced, T *actual, QString exp = QString())
    {
        if(notExpeced == actual)
            ThrowError(QString("Assert::ArePtrNotEqual failure."),exp);
    }

    void IsTrue(bool state, QString exp = QString())
    {
        if(!state)
            ThrowError(QString("Assert::IsTrue failure."),exp);
    }

    void IsFalse(bool state, QString exp = QString())
    {
        if(state)
            ThrowError(QString("Assert::IsFale failure."),exp);
    }

    template <typename T>
    void IsPtr(T * ptr, QString exp = QString())
    {
        if(ptr == nullptr)
            ThrowError(QString("Assert::IsPtr failure."),exp);
    }

    template <typename T>
    void IsNullPtr(T * ptr, QString exp = QString())
    {
        if(ptr != nullptr)
            ThrowError(QString("Assert::IsNullPtr failure."),exp);
    }

    void Message(QString message)
    {
        ThrowError(message);
    }

private:
    int _line;
    void ThrowError(QString message, QString exp = QString());
};


#define Assert _Assert(__LINE__)
#define when( ... ) (__VA_ARGS__, #__VA_ARGS__)
#define _exp( ... ) __VA_ARGS__, #__VA_ARGS__

#define TEST_ADD(_TEST_CLASS, ... ) mibot::UnitTestManager::RegTest( #_TEST_CLASS , new _TEST_CLASS( __VA_ARGS__ ) );
#define TEST_FLUSH() mibot::UnitTestManager::Flush()

#define TEST_RES_PRINT(_RES) mibot::UnitTestManager::ResultsPtr( _RES );
#define TEST_RES_FILE(_RES, _FNAME) mibot::UnitTestManager::ResultsToTextFile( _FNAME, _RES );
}
#endif // UNITTEST_H
