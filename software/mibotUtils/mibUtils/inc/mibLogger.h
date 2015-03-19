#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "mibUtilsGlobal.h"
#include "mibLoggerChannel.h"

#include "mibLoggerStandardFormaters.h"
#include "mibLoggerStandardOutputs.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT LoggerManager final
{
public:
    LoggerManager();

    ~LoggerManager();
    static LoggerManager *instance();

    LoggerChannel *GetChannel(QString name);
    LoggerChannel *GetDefaultChannel();

    void AddChannel(QString name, LoggerChannel * channel);
    void SelectDefaultChannel(QString name);

private:

    QMap<QString, LoggerChannel*> _channels;
    LoggerChannel* _default_channel;
};

}
#endif // LOGGER_H
