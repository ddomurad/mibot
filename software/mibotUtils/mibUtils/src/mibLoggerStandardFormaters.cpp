#include "inc/mibLoggerStandardFormaters.h"

using namespace mibot;

LoggerSimpleStringFormater::LoggerSimpleStringFormater()
{}

LoggerSimpleStringFormater::~LoggerSimpleStringFormater()
{}

QString LoggerSimpleStringFormater::LogLevel2Str(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Error: return "ERROR";
        case LogLevel::ImportantInfo: return "IMPORTANT";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
    }

    return "UNKNOWN";
}

LogLevel LoggerSimpleStringFormater::Str2LogLevel(QString str)
{
    if(str.toLower() == "debug") return LogLevel::Debug;
    if(str.toLower() == "error") return LogLevel::Error;
    if(str.toLower() == "important") return LogLevel::ImportantInfo;
    if(str.toLower() == "info") return LogLevel::Info;
    if(str.toLower() == "warning") return LogLevel::Warning;

    return LogLevel::Error;
}


QString LoggerSimpleStringFormater::FormatMessage(
        LogLevel level, QString file,
        QString function, qint32 line,
        QString message)
{
    int index = file.lastIndexOf('\\');
    if(index == -1) index = file.lastIndexOf('/');
    if(index != -1) file = file.right(file.size() - index - 1);
    return QString("[%1] at (%2: %3: %4): %5\n")
            .arg( LogLevel2Str(level), file, function, QString::number(line), message);
}


LoggerSimpleConsoleFormater::LoggerSimpleConsoleFormater()
{}

LoggerSimpleConsoleFormater::~LoggerSimpleConsoleFormater()
{}

QString LoggerSimpleConsoleFormater::FormatMessage(
        LogLevel level, QString file,
        QString function, qint32 line,
        QString message)
{
    _message_count++;
    int index = file.lastIndexOf('\\');
    if(index == -1) index = file.lastIndexOf('/');
    if(index != -1) file = file.right(file.size() - index - 1);
    return QString("[%6][%1] at (%2: %3: %4): %5")
            .arg( LoggerSimpleStringFormater::LogLevel2Str(level),
                  file, function, QString::number(line), message)
            .arg(_message_count);
}

uint LoggerSimpleConsoleFormater::_message_count = 0;
