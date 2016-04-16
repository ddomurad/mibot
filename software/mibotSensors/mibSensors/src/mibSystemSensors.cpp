#include "inc/mibSystemSensors.h"
#include <mibLogger.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/sysinfo.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace mibot;

SystemSensors::SystemSensors()
{
    lastReadingElapsed.start();
    _isInitialized = false;
}

SystemSensors::~SystemSensors()
{}

SystemSensors *SystemSensors::get()
{
    static SystemSensors sensor;
    return &sensor;
}

SystemSensorsReading SystemSensors::Readings()
{
    if(!_isInitialized)
        return _readings;

    QMutexLocker locket(&_mutex);
    _updateReadsIfNeeded();

    return _readings;
}

bool SystemSensors::Initialize()
{
    QMutexLocker locket(&_mutex);
    if(_isInitialized)
        return true;

    _settings = SensorsSettings::getGlobal();
    if(!_settings->Sync())
    {
        LOG_ERROR("Can't sync sensor settings.");
        _settings->Release();
        return false;
    }

    _ram_available = 0;
    _last_cpu_idel = 0;
    _last_process_stime = 0;
    _last_process_utime = 0;
    _last_process_cutime = 0;
    _last_process_cstime = 0;

    _cpu_process_cpu_path = QString("/proc/%1/stat").arg(getpid());
    _process_status_path = QString("/proc/%1/status").arg(getpid());

    calcCpuCount();
    if(_cpu_count == 0)
    {
        LOG_ERROR("Can't detect any cpu.");
        return false;
    }

    _isInitialized = true;
    return true;
}

void SystemSensors::_updateReadsIfNeeded()
{
    if(lastReadingElapsed.elapsed() > _settings->systemSensorsUpdateTime->value)
    {
        readAllSensors();
        lastReadingElapsed.restart();
    }
}

void SystemSensors::readAllSensors()
{
    float val = readSystemStateValue( _cpu_temp_path , 64).toDouble() * 0.001;
    _readings.cpu_temp = val;

    float cpu_total = 0.0f;
    float cpu_server = 0.0f;

    readCpuUtilization(&cpu_total, &cpu_server);

    _readings.cpu_usage_server = cpu_server;
    _readings.cpu_usage_total = cpu_total;

    float avaiable = 0;
    float used_total = 0;
    float used_process = 0;

    readRamUtilization( &avaiable, &used_total, &used_process );

    _readings.mem_available = avaiable;
    _readings.mem_usage_total = used_total;
    _readings.mem_usage_server = used_process;
    _readings.datetime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
}

QString SystemSensors::readSystemStateValue(QString path, int length)
{
    int fd;
    if ((fd = open(path.toStdString().c_str(), O_RDONLY)) == -1)
    {
        return "";
    }

    char *buf = new char[length];
    memset( buf, 0, length );

    if ( read(fd, buf, length) == -1 )
    {
        delete[] buf;
        close(fd);
        return "";
    }

    close(fd);
    QString ret = QString(buf);
    delete[] buf;
    return ret;
}

QString SystemSensors::readSystemStateLine(QString path)
{
    std::fstream input(path.toStdString(),std::ios::in);

    if(!input.is_open())
        return "";

    std::string line;
    if(!std::getline(input, line))
    {
        input.close();
        return "";
    }

    input.close();
    return QString(line.c_str());
}

void SystemSensors::readCpuUtilization(float *cpu_total, float *cpu_server)
{
    float elapsed = float(_cpu_reading_timer.restart());
    QString cpu_line = readSystemStateLine(_cpu_state_path);
    QString server_line = readSystemStateLine(_cpu_process_cpu_path);

    if(cpu_line.isEmpty())
        return;

    if( cpu_line.count() < 15 || cpu_line.left(4) != "cpu ")
        return;

    unsigned int cpu_itime;
    unsigned int cpu_utime;
    unsigned int cpu_stime;

    unsigned int process_utime;
    unsigned int process_stime;
    unsigned int process_cutime;
    unsigned int process_cstime;

    sscanf(cpu_line.toStdString().c_str(), "%*s %u %*s %u %u", &cpu_utime, &cpu_stime, &cpu_itime);
    sscanf(server_line.toStdString().c_str(), "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %u %u %u %u",
           &process_utime, &process_stime,
           &process_cutime, &process_cstime);

    *cpu_total = 1.0f - (10.0f*float( cpu_itime - _last_cpu_idel)/float(elapsed))/float(_cpu_count);

    float serv_cpu = 10.0*float(process_utime - _last_process_utime)/elapsed;
    serv_cpu += 10.0*float(process_stime - _last_process_stime)/elapsed;
    serv_cpu += 10.0*float(process_cutime - _last_process_cutime)/elapsed;
    serv_cpu += 10.0*float(process_cstime - _last_process_cstime)/elapsed;

    serv_cpu /= float(_cpu_count);

    *cpu_server = serv_cpu;

    _last_cpu_idel = cpu_itime;
    _last_process_stime = process_stime;
    _last_process_utime = process_utime;
    _last_process_cstime = process_cstime;
    _last_process_cutime = process_cutime;
}

void SystemSensors::readRamUtilization(float *avaiable, float *used_total, float *used_process)
{
    struct sysinfo memInfo;
    sysinfo (&memInfo);

    if(_ram_available == 0)
    {
        _ram_available = memInfo.totalram * memInfo.mem_unit;
    }
    *avaiable = float(_ram_available)/(1024.0f*1024.0f);

    *used_total = float((memInfo.totalram - memInfo.freeram)*memInfo.mem_unit)/(1024.0f*1024.0f);

    std::fstream input(_process_status_path.toStdString(),std::ios::in);

    if(!input.is_open())
        return;

    std::string l;

    while(std::getline(input, l))
    {
        QString line(l.c_str());
        if(line.startsWith("VmRSS"))
        {
            QString num;
            for(int i=0;i<line.length(); i++)
                if(line[i] >= '0' && line[i] <= '9')
                    num += line[i];
            *used_process = num.toFloat() / 1024.0f;

            break;
        }
    }

    input.close();
}

void SystemSensors::calcCpuCount()
{
    QString cpu_state = readSystemStateValue(_cpu_state_path, 512);
    _cpu_count = 0;
    if(cpu_state.isEmpty()) return;

    while(true)
    {
        if(cpu_state.contains( QString("cpu%1").arg(_cpu_count) ))
            _cpu_count++;
        else break;
    }
}


QString SystemSensors::_cpu_temp_path = "/sys/class/thermal/thermal_zone0/temp";
QString SystemSensors::_cpu_state_path = "/proc/stat";
