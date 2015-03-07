#ifndef LOGGERFORMATER_H
#define LOGGERFORMATER_H

#include <QString>
#include "mibLogger_global.h"

namespace mibot
{

class LOGGERSHARED_EXPORT LoggerFormater
{
public:
    virtual ~LoggerFormater(){}
    virtual QString FormatMessage(
            LogLevel level, QString file,
            QString function, qint32 line,
            QString message) = 0;
};

}
#endif // LOGGERFORMATER_H
