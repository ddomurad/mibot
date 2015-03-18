#include "LoggerUnitTests.h"

using namespace mibot;
using namespace mibot::ut;

MockLoggerOutput::MockLoggerOutput():
    LoggerOutput(nullptr),
    WriteCallsCount(0)
{}

void MockLoggerOutput::Write(QString message)
{
    WriteCallsCount++;
    CallArguments.append(message);
}

void MockLoggerOutput::WriteLog(LogLevel, QString, QString , qint32 , QString message)
{
    Write(message);
}


LoggerUnitTests::LoggerUnitTests() :
    QObject(nullptr)
{}

LoggerUnitTests::~LoggerUnitTests()
{}

void LoggerUnitTests::TestInit()
{
    loggerManager = new LoggerManager();
}

void LoggerUnitTests::TestClean()
{
    delete loggerManager;
}

void LoggerUnitTests::CanCreateChannel()
{
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName) );
    loggerManager->AddChannel( testChannelName, new LoggerChannel(LogLevel::Debug) );
    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName) );
}

void LoggerUnitTests::CanCreateMultipleChannels()
{
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "1") );
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "2") );

    loggerManager->AddChannel( testChannelName + "1", new LoggerChannel(LogLevel::Debug) );

    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "1") );
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "2") );

    loggerManager->AddChannel( testChannelName + "2", new LoggerChannel(LogLevel::Debug) );

    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "1") );
    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "2") );
}

void LoggerUnitTests::FirstCreatedChannelIsSetAsDefault()
{
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName) );
    Assert.IsNullPtr<LoggerChannel> when( loggerManager->GetDefaultChannel() );

    auto channel = new LoggerChannel(LogLevel::Debug);
    loggerManager->AddChannel(testChannelName, channel);

    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName) );
    Assert.IsPtr<LoggerChannel> when( loggerManager->GetDefaultChannel() );

    Assert.ArePtrEqual<LoggerChannel> when( loggerManager->GetChannel(testChannelName),
                                       loggerManager->GetDefaultChannel() );

}

void LoggerUnitTests::DefaultChannelCanBeChanged()
{
    auto channel = new LoggerChannel(LogLevel::Debug);
    auto channel2 = new LoggerChannel(LogLevel::Debug);
    loggerManager->AddChannel( testChannelName, channel );
    loggerManager->AddChannel( testChannelName + "2", channel2 );

    Assert.IsPtr<LoggerChannel> when( loggerManager->GetChannel(testChannelName) );
    Assert.IsPtr<LoggerChannel> when( loggerManager->GetDefaultChannel() );

    Assert.ArePtrEqual<LoggerChannel> when( loggerManager->GetChannel(testChannelName),
                                       loggerManager->GetDefaultChannel());

    loggerManager->SelectDefaultChannel( testChannelName + "2" );

    Assert.ArePtrEqual<LoggerChannel> when( loggerManager->GetChannel(testChannelName + "2"),
                                       loggerManager->GetDefaultChannel());

}

void LoggerUnitTests::MessageIsWritenToOutput()
{
    LoggerChannel channel(LogLevel::Debug);
    MockLoggerOutput * outputMock = new MockLoggerOutput();

    QString testMessage[] = {"test message", "some other message"};

    channel += outputMock ;

    Assert.AreEqual<int> when( 0, outputMock->WriteCallsCount );

    channel.WriteMessage( testMessage[0] );
    channel.WriteMessage( testMessage[1] );

    Assert.AreEqual<int> when( 2, outputMock->WriteCallsCount );
    Assert.AreEqual<QString> when( testMessage[0], outputMock->CallArguments[0] );
    Assert.AreEqual<QString> when( testMessage[1], outputMock->CallArguments[1] );
}

void LoggerUnitTests::MessageIsWritenToOutputWhenTheLevelIsHigher()
{
    LoggerChannel channel(LogLevel::ImportantInfo);
    MockLoggerOutput * outputMock = new MockLoggerOutput();

    QString testMessage = "test message";

    channel += outputMock ;

    Assert.AreEqual<int> when( 0, outputMock->WriteCallsCount );

    channel.WriteLog(LogLevel::Error, "file", "function", 0, "msg");

    Assert.AreEqual<int> when( 1, outputMock->WriteCallsCount );
}

void LoggerUnitTests::MessageIsNOTWritenToOutputWhenTheLevelIsLower()
{
    LoggerChannel channel(LogLevel::ImportantInfo);
    MockLoggerOutput * outputMock = new MockLoggerOutput();

    QString testMessage = "test message";

    channel += outputMock ;

    Assert.AreEqual<int> when( 0, outputMock->WriteCallsCount );

    channel.WriteLog(LogLevel::Info, "file", "function", 0, "msg");

    Assert.AreEqual<int> when( 0, outputMock->WriteCallsCount );
}

void LoggerUnitTests::FileOutputCorectlyWriteMessagesToFile()
{
    QString testTempFile = "./tmp_log_file";
    QString testMessage = "some fancy test message";

    if(QFile::exists(testTempFile))
        Assert.IsTrue when( QFile::remove(testTempFile) );

    LoggerChannel channel(LogLevel::Debug);
    LoggerFileOutput * foutput = new LoggerFileOutput(testTempFile);
    channel += foutput;

    channel.WriteMessage(testMessage );

    Assert.IsTrue when(QFile::exists(testTempFile));

    QFile inputTestFile(testTempFile);
    Assert.IsTrue when(inputTestFile.open(QIODevice::ReadOnly));
    QByteArray fileContent = inputTestFile.readAll();
    inputTestFile.close();

    QString fileStringContent = QString( fileContent );

    Assert.AreEqual<QString> when(testMessage ,fileStringContent );
    Assert.IsTrue when( QFile::remove(testTempFile) );
}

void LoggerUnitTests::SimpleFormaterCorrectlyTranslateLogLevels()
{
    Assert.AreEqual<QString> when( "DEBUG", LoggerSimpleStringFormater::LogLevel2Str( LogLevel::Debug ) );
    Assert.AreEqual<QString> when( "ERROR", LoggerSimpleStringFormater::LogLevel2Str( LogLevel::Error ) );
    Assert.AreEqual<QString> when( "IMPORTANT", LoggerSimpleStringFormater::LogLevel2Str( LogLevel::ImportantInfo ) );
    Assert.AreEqual<QString> when( "INFO", LoggerSimpleStringFormater::LogLevel2Str( LogLevel::Info ) );
    Assert.AreEqual<QString> when( "WARNING", LoggerSimpleStringFormater::LogLevel2Str( LogLevel::Warning ) );

    Assert.AreEqualNP<LogLevel> when( LogLevel::Debug, LoggerSimpleStringFormater::Str2LogLevel("debug") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Error, LoggerSimpleStringFormater::Str2LogLevel("error") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::ImportantInfo, LoggerSimpleStringFormater::Str2LogLevel("important") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Info, LoggerSimpleStringFormater::Str2LogLevel("info") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Warning, LoggerSimpleStringFormater::Str2LogLevel("warning") );

    Assert.AreEqualNP<LogLevel> when( LogLevel::Debug, LoggerSimpleStringFormater::Str2LogLevel("debuG") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Error, LoggerSimpleStringFormater::Str2LogLevel("eRRor") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::ImportantInfo, LoggerSimpleStringFormater::Str2LogLevel("iMPortaNT") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Info, LoggerSimpleStringFormater::Str2LogLevel("INFo") );
    Assert.AreEqualNP<LogLevel> when( LogLevel::Warning, LoggerSimpleStringFormater::Str2LogLevel("WARninG") );
}

void LoggerUnitTests::SimpleFormaterCorrectlyFormatsMessages()
{
    LoggerSimpleStringFormater formater = LoggerSimpleStringFormater();
    QString expected = "[DEBUG] at (file: function: 123): message\n";
    QString fmessage = formater.FormatMessage(LogLevel::Debug,"file","function",123,"message");

    Assert.AreEqual<QString> when(expected, fmessage);
}

void LoggerUnitTests::SimpleFormaterCorrectlyFormatsFilePath()
{
    LoggerSimpleStringFormater formater = LoggerSimpleStringFormater();
    QString expected1 = "[DEBUG] at (file: function: 123): message\n";
    QString expected2 = "[DEBUG] at (file: function: 123): message\n";

    QString fmessage1 = formater.FormatMessage(LogLevel::Debug,"start\\prefix\\file","function",123,"message");
    QString fmessage2 = formater.FormatMessage(LogLevel::Debug,"start/prefix/file","function",123,"message");

    Assert.AreEqual<QString> when(expected1, fmessage1);
    Assert.AreEqual<QString> when(expected2, fmessage2);
}

void LoggerUnitTests::SimpleConsoleFormaterCorrectlyFormatsMessages()
{
    LoggerSimpleConsoleFormater formater = LoggerSimpleConsoleFormater();
    QString expected1 = "[1][DEBUG] at (file: function: 123): message\n";
    QString expected2 = "[2][INFO] at (file: function: 123): message\n";
    QString expected3 = "[3][ERROR] at (file: function: 123): message\n";

    QString fmessage1 = formater.FormatMessage(LogLevel::Debug,"file","function",123,"message");
    QString fmessage2 = formater.FormatMessage(LogLevel::Info,"file","function",123,"message");
    QString fmessage3 = formater.FormatMessage(LogLevel::Error,"file","function",123,"message");

    Assert.AreEqual<QString> when(expected1, fmessage1);
    Assert.AreEqual<QString> when(expected2, fmessage2);
    Assert.AreEqual<QString> when(expected3, fmessage3);
}
