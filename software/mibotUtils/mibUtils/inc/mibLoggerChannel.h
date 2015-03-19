#ifndef LOGGERCHANNEL_H
#define LOGGERCHANNEL_H

#include <QtCore>
#include <initializer_list>

#include "mibUtilsGlobal.h"
#include "mibLoggerOutput.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerChannel final
{
public:
    LoggerChannel(LogLevel level);
    LoggerChannel(LogLevel level, std::initializer_list<LoggerOutput*>);
    ~LoggerChannel();

    void Append(std::initializer_list<LoggerOutput*>);

    void operator += (LoggerOutput*);
    void operator += (std::initializer_list<LoggerOutput*>);

    void WriteLog(LogLevel level, QString file, QString function, uint line, QString message);
    void WriteMessage(QString msg);

private:
    LogLevel _level;
    QList<LoggerOutput*> _outputs;
};

}
#endif // LOGGERCHANNEL_H
