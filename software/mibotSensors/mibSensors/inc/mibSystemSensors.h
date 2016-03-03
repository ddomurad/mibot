#ifndef SYSTEMSENSORS_H
#define SYSTEMSENSORS_H

#include <QThread>
#include "mibSensorsGlobal.h"
#include "mibSensorsSettings.h"

namespace mibot
{

class MIBSENSORSSHARED_EXPORT SystemSensorsReading
{
public:

    float cpu_temp;
    float cpu_usage_total;
    float cpu_usage_server;
    float mem_available;
    float mem_usage_total;
    float mem_usage_server;
};

class MIBSENSORSSHARED_EXPORT SystemSensors
{
public:
    ~SystemSensors();

    static SystemSensors * get();
    SystemSensorsReading Readings();
    bool Initialize();
private:

    SystemSensors();
    void _updateReadsIfNeeded();
    QMap<QString, QVariant> getLastReads();

    QElapsedTimer lastReadingElapsed;
    void readAllSensors();
    QString readSystemStateValue(QString path, int length);
    QString readSystemStateLine(QString path);

    void readCpuUtilization(float *cpu_total, float *cpu_server);
    void readRamUtilization(float *available, float *used_total, float *used_process);
    void calcCpuCount();

    unsigned int _ram_available;
    unsigned int _cpu_count;
    unsigned int _last_cpu_idel;
    unsigned int _last_process_utime;
    unsigned int _last_process_stime;
    unsigned int _last_process_cutime;
    unsigned int _last_process_cstime;

    QElapsedTimer _cpu_reading_timer;

    static QString _cpu_temp_path;
    static QString _cpu_state_path;
    QString _cpu_process_cpu_path;
    QString _process_status_path;

    SystemSensorsReading _readings;
    SensorsSettings * _settings;
    QMutex _mutex;

    bool _isInitialized;
};

}
#endif // SYSTEMSENSORS_H
