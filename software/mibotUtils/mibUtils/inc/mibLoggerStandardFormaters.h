#ifndef LOGGERSIMPLESTRINGFORMATER_H
#define LOGGERSIMPLESTRINGFORMATER_H

#include "mibUtilsGlobal.h"
#include "mibLoggerFormater.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerSimpleStringFormater : public LoggerFormater
{
public:
    LoggerSimpleStringFormater();
    ~LoggerSimpleStringFormater();

    static QString LogLevel2Str(LogLevel level);
    static LogLevel Str2LogLevel(QString str);

    // LoggerFormater interface
public:
    QString FormatMessage(LogLevel level, QString file, QString function, qint32 line, QString message);
};

class MIBUTILSSHARED_EXPORT LoggerSimpleConsoleFormater : public LoggerFormater
{
public:
    LoggerSimpleConsoleFormater();
    ~LoggerSimpleConsoleFormater();

    // LoggerFormater interface
public:
    QString FormatMessage(LogLevel level, QString file, QString function, qint32 line, QString message);

private:
    static uint _message_count;
};

}
#endif // LOGGERSIMPLESTRINGFORMATER_H
