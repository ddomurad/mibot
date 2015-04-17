#ifndef STATUSREADER_H
#define STATUSREADER_H

#include <QtCore>
#include <QTimer>
#include <QElapsedTimer>
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
    const QString CpuUsageGeneral = "cpu_usage_general";
    const QString CpuUsageServer = "cpu_usage_server";
    const QString MemAvailable = "mem_available";
    const QString MemUsageGeneral = "mem_usage_general";
    const QString MemUsageServer = "mem_usage_server";

private slots:
    void onRefreshReadings();

private:
    StatusReader(StatusConfigRes *cfg);
    QTimer * _timer;
    StatusConfigRes * _cfg;
    MCP3008 * _mcp3008;
    QMap<QString, QVariant> _readings;

    QString readSystemStateValue(QString path, int length);
    QString readSystemStateLine(QString path);
    void readCpuUtilization(float *cpu_total, float *cpu_server);
    void calcCpuCount();

    unsigned int _cpu_count;
    unsigned int _last_cpu_idel;
    unsigned int _last_process_utime;
    unsigned int _last_process_stime;
    unsigned int _last_process_cutime;
    unsigned int _last_process_cstime;

    QElapsedTimer _cpu_reading_timer;

    int    _ref_counter;
    static QMutex _mutex;
    static QMap<QUuid, StatusReader*> _readers;

    static QString _cpu_temp_path;
    static QString _cpu_state_path;
    QString _cpu_process_cpu_path;
};

}

#endif // STATUSREADER_H
