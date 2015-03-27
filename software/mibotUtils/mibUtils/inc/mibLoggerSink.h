#ifndef LOGGEROUTPUT_H
#define LOGGEROUTPUT_H

#include <QtCore>
#include "mibUtilsGlobal.h"
#include "mibLoggerFormater.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerSink
{
public:
    explicit LoggerSink(LogLevel level, LoggerFormater * formater);
    virtual ~LoggerSink();

    virtual void Write(QString message) = 0;
    virtual void WriteLog(LogLevel level, QString file, QString function, qint32 line, QString message);

    LogLevel      GetLevel();
private:
    LogLevel         _level;
    LoggerFormater * _formater;
};

}
#endif // LOGGEROUTPUT_H
