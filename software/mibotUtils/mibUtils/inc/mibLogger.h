#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "mibUtilsGlobal.h"
#include "mibLoggerSink.h"

#include "mibLoggerStandardFormaters.h"
#include "mibLoggerStandardSinks.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerManager final
{
public:
    LoggerManager();

    ~LoggerManager();
    static LoggerManager *instance();

    void WriteLog(LogLevel level, QString file, QString function, uint line, QString message);
    void WriteMessage(QString msg);

    void AddSink(LoggerSink * sink);

private:

    QList<LoggerSink*> _sinks;
};

}
#endif // LOGGER_H
