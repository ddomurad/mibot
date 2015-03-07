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


QString mibot::LoggerSimpleStringFormater::FormatMessage(
        mibot::LogLevel level, QString file,
        QString function, qint32 line,
        QString message)
{
    int index = file.lastIndexOf('\\');
    if(index == -1) index = file.lastIndexOf('/');
    if(index != -1) file = file.right(file.size() - index - 1);
    return QString("[%1] at (%2: %3: %4): %5\n").arg( LogLevel2Str(level), file, function, QString::number(line), message);
}
