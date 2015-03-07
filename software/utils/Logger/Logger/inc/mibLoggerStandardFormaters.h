#ifndef LOGGERSIMPLESTRINGFORMATER_H
#define LOGGERSIMPLESTRINGFORMATER_H

#include "mibLogger_global.h"
#include "mibLoggerFormater.h"

namespace mibot
{

class LoggerSimpleStringFormater : public LoggerFormater
{
public:
    LoggerSimpleStringFormater();
    ~LoggerSimpleStringFormater();

    static QString LogLevel2Str(LogLevel level);

    // LoggerFormater interface
public:
    QString FormatMessage(LogLevel level, QString file, QString function, qint32 line, QString message);
};

}
#endif // LOGGERSIMPLESTRINGFORMATER_H
