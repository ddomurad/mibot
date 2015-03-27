#ifndef LOGGERBUILDER_H
#define LOGGERBUILDER_H

#include "mibUtilsGlobal.h"
#include "mibLoggerSink.h"

#include <QJsonObject>

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerBuilder
{
public:
    enum class LOG_TYPE { OK, ERROR };
    struct LogEntry
    {
        LOG_TYPE type;
        QString message;
    };

    virtual ~LoggerBuilder(){}
    virtual void BuildLogger(QJsonObject &json) = 0;

    void LogProcess(LOG_TYPE type, QString message);
    QList<LogEntry> GetBuildLog(bool onlyErrors);
    bool AreErrors();

private:
    QList<LogEntry> _log;
};

}
#endif // LOGGERBUILDER_H
