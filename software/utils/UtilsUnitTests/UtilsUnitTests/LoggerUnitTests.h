#ifndef LOGGERUNITTESTS_H
#define LOGGERUNITTESTS_H

#include <mibLogger.h>

#include <QObject>
#include <mibUnitTest.h>

namespace mibot
{
namespace ut
{

class MockLoggerOutput : public LoggerOutput
{
public:
    MockLoggerOutput();

    int WriteCallsCount;
    QList<QString> CallArguments;

    void Write(QString message);
    void WriteLog(LogLevel, QString, QString, qint32, QString message);
};



class LoggerUnitTests : public QObject, public UnitTest
{
    Q_OBJECT
public:
    LoggerUnitTests();
    ~LoggerUnitTests();
private:
    LoggerManager * loggerManager;

    const QString testChannelName = "testChannel";

public slots:
    void CanCreateChannel();
    void CanCreateMultipleChannels();
    void FirstCreatedChannelIsSetAsDefault();
    void DefaultChannelCanBeChanged();

    void MessageIsWritenToOutput();
    void MessageIsWritenToOutputWhenTheLevelIsHigher();
    void MessageIsNOTWritenToOutputWhenTheLevelIsLower();


    void FileOutputCorectlyWriteMessagesToFile();

    void SimpleFormaterCorrectlyTranslateLogLevels();
    void SimpleFormaterCorrectlyFormatsMessages();
    void SimpleFormaterCorrectlyFormatsFilePath();
    void SimpleConsoleFormaterCorrectlyFormatsMessages();

    // UnitTest interface
public:
    void TestInit();
    void TestClean();
};

}
}

#endif // LOGGERUNITTESTS_H
