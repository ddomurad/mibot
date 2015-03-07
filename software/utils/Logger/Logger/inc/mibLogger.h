#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include "mibLogger_global.h"
#include "mibLoggerChannel.h"

#include "mibLoggerStandardFormaters.h"
#include "mibLoggerStandardOutputs.h"

namespace mibot
{

class LOGGERSHARED_EXPORT LoggerManager final
{

public:
    ~LoggerManager();
    static LoggerManager *instance();

    static LoggerChannel *GetChannel(QString name);
    static LoggerChannel *GetDefaultChannel();

    static void AddChannel(QString name, LoggerChannel * channel);
    static void SelectDefaultChannel(QString name);

private:
    LoggerManager();
    QMap<QString, LoggerChannel*> _channels;
    static LoggerChannel* _default_channel;

    static QString _given_channel_dont_exists;
};

}
#endif // LOGGER_H
