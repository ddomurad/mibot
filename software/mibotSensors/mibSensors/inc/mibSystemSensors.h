#ifndef SYSTEMSENSORS_H
#define SYSTEMSENSORS_H

#include "mibSensor.h"

namespace mibot
{

class MIBSENSORSSHARED_EXPORT SystemSensors : public Sensor<QString>
{
public:
    ~SystemSensors();

    static SystemSensors * get();
    const QString EnginesAccuVoltage = "accu_volt";
    const QString CpuTemperature = "cpu_temp";
    const QString CpuUsageTotal = "cpu_usage_total";
    const QString CpuUsageServer = "cpu_usage_server";
    const QString MemAvailable = "mem_available";
    const QString MemUsageTotal = "mem_usage_total";
    const QString MemUsageServer = "mem_usage_server";
    // Sensor interface
protected:

    bool _intialize();
    void _readAllSensors();

private:

    SystemSensors();
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
};

}
#endif // SYSTEMSENSORS_H
