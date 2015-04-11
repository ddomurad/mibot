#ifndef STATUSREADER_H
#define STATUSREADER_H

#include <QtCore>
#include <QTimer>
#include <QMutex>

#include <mibMCP3008.h>
#include <mibConnection.h>

#include "mibStatusStrategyGlobal.h"
#include "mibStatusConfigRes.h"

namespace mibot
{

class MIBSTATUSSTRATEGYSHARED_EXPORT  StatusReader : public QObject
{
    Q_OBJECT
public:
    static StatusReader * GetStatusReader(Connection * connection);
    static void ReleaseStatusReader(Connection * connection);

    ~StatusReader();

    bool Init();

    QMap<QString, QVariant> Readings();
    StatusConfigRes * Config();

    const QString EnginesAccuVoltage = "accu_volt";
    const QString CpuTemperature = "cpu_temp";
private slots:
    void onRefreshReadings();

private:
    StatusReader(StatusConfigRes *cfg);
    QTimer * _timer;
    StatusConfigRes * _cfg;
    MCP3008 * _mcp3008;
    QMap<QString, QVariant> _readings;

    QString readSystemStateValue(QString path);
    const int __BUF_SIZE = 64;

    int    _ref_counter;
    static QMutex _mutex;
    static QMap<QUuid, StatusReader*> _readers;
};

}

#endif // STATUSREADER_H
