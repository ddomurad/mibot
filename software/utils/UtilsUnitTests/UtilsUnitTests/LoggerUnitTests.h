#ifndef LOGGERUNITTESTS_H
#define LOGGERUNITTESTS_H

#include <mibLogger.h>

#include <QObject>
#include <mibUnitTest.h>

namespace mibot
{
namespace ut
{

class MockLoggerChannel : public LoggerOutput
{
public:
    MockLoggerChannel();

    int WriteCallsCount;
    QList<QString> CallArguments;

    void Write(QString message);
};



class LoggerUnitTests : public QObject, public UnitTest
{
    Q_OBJECT
public:
    LoggerUnitTests();
    ~LoggerUnitTests();

public slots:
    void testTestosteron();
    void testTestosteron2();
};

}
}
#endif // LOGGERUNITTESTS_H
