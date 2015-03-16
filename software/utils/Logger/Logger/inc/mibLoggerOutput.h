#ifndef LOGGEROUTPUT_H
#define LOGGEROUTPUT_H

#include <QtCore>
#include "mibLogger_global.h"
#include "mibLoggerFormater.h"

namespace mibot
{

class LOGGERSHARED_EXPORT LoggerOutput
{
public:
    explicit LoggerOutput(LoggerFormater * formater);
    virtual ~LoggerOutput();

    virtual void Write(QString message) = 0;

    virtual void WriteLog(LogLevel level, QString file, QString function, qint32 line, QString message);

private:
    LoggerFormater * _formater;
};

}
#endif // LOGGEROUTPUT_H
