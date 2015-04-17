#include "inc/mibStatusReader.h"
#include <mibLogger.h>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace mibot;

StatusReader::StatusReader(StatusConfigRes *cfg) :
    QObject(nullptr), _cfg(cfg), _mcp3008(nullptr),
    _ref_counter(0)
{
    _last_cpu_idel = 0;
    _last_process_stime = 0;
    _last_process_utime = 0;
    _last_process_cutime = 0;
    _last_process_cstime = 0;

    _cpu_reading_timer.start();

    _cpu_process_cpu_path = QString("/proc/%1/stat").arg(getpid());

    _timer = new QTimer(this);
    connect(_timer,SIGNAL(timeout()),this,SLOT(onRefreshReadings()));
}

StatusReader *StatusReader::GetStatusReader(Connection *connection)
{
    QMutexLocker locker(&_mutex);
    if(_readers.contains( connection->SocketObj()->Id() ))
    {
        _readers[connection->SocketObj()->Id() ]->_ref_counter ++;
        return _readers[connection->SocketObj()->Id() ];
    }

    ResourceWrapper<StatusConfigRes> *repo = new ResourceWrapper<StatusConfigRes>(GlobalAccess::get().Repository());
    StatusConfigRes * cfg = repo->getByID( connection->SocketObj()->StrategyConfig() );
    delete repo;

    if(cfg == nullptr)
    {
        LOG_ERROR( QString("Can't read StateStrategy configuration id: %1").arg(connection->SocketObj()->StrategyConfig()));

        _readers.insert( connection->SocketObj()->Id(), nullptr );
        return nullptr;
    }

    StatusReader * reader = new StatusReader(cfg);
    reader->_ref_counter = 1;

    if(reader->Init() == false)
    {
        LOG_ERROR(QString("Failed to initialize StatusReader with config: '%1'").arg(cfg->Alias()));
        _readers.insert( connection->SocketObj()->Id(), nullptr );
        return nullptr;
    }

    _readers.insert( connection->SocketObj()->Id(), reader );

    return reader;
}

void StatusReader::ReleaseStatusReader(Connection *connection)
{
    QMutexLocker locker(&_mutex);
    if(_readers.contains( connection->SocketObj()->Id() ))
    {
        _readers[connection->SocketObj()->Id() ]->_ref_counter --;
        if(_readers[connection->SocketObj()->Id() ]->_ref_counter == 0)
        {
            delete _readers[connection->SocketObj()->Id() ];
            _readers.remove( connection->SocketObj()->Id() );
        }
    }
}

StatusReader::~StatusReader()
{
    if(_mcp3008 != nullptr)
        delete _mcp3008;
}

bool StatusReader::Init()
{
    calcCpuCount();
    if(_cpu_count == 0)
    {
        LOG_ERROR("Can't detect any cpu.");
        return false;
    }

    _timer->start( _cfg->UpdateRatio() );

    if(_cfg->UseMcp3008())
    {
        LOG_IMPORTANT( QString("Initializing mcp3008 (base: %1, channel: %2)")
                       .arg(_cfg->Mcp3008Base())
                       .arg(_cfg->Mcp3008SpiChannel()));

        _mcp3008 = new MCP3008();
        if(_mcp3008->Init( _cfg->Mcp3008Base(), _cfg->Mcp3008SpiChannel()))
        {
            LOG_IMPORTANT("Mcp3008 spi channel inited.");
        }
        else
        {
            LOG_ERROR("Failed to initialize mcp3008 spi.");
            return false;
        }

    }else
    {
        if(_cfg->ReadEnginesAccu())
        {
            LOG_WARNING("Can't read engines accumulator state when mcp3008 is disabled.");
        }
    }

    return true;
}


QMap<QString, QVariant> StatusReader::Readings()
{
    QMutexLocker locker(&_mutex);
    return _readings;
}

StatusConfigRes *StatusReader::Config()
{
    return _cfg;
}

void StatusReader::onRefreshReadings()
{
    QMutexLocker locker(&_mutex);
    _readings.clear();
    if(_cfg->UseMcp3008())
    {
        if(_cfg->ReadEnginesAccu())
        {
            int val = _mcp3008->ReadChannel( _cfg->EnginesAccuMcp3008Channel() );
            double cval = double (val) * _cfg->EnginesAccuVScale();
            _readings.insert( EnginesAccuVoltage, QVariant(cval) );
        }
    }
    if(_cfg->ReadCpuTemp())
    {
        double val = readSystemStateValue( _cpu_temp_path , 64).toDouble() * _cfg->CpuTempScale();
        _readings.insert( CpuTemperature , QVariant(val) );
    }

    float cpu_total = 0.0f;
    float cpu_server = 0.0f;

    readCpuUtilization(&cpu_total, &cpu_server);

    _readings.insert( CpuUsageGeneral , QVariant( cpu_total ) );
    _readings.insert( CpuUsageServer, QVariant( cpu_server ) );
}

QString StatusReader::readSystemStateValue(QString path, int length)
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

QString StatusReader::readSystemStateLine(QString path)
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

void StatusReader::readCpuUtilization(float * cpu_total, float * cpu_server)
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

void StatusReader::calcCpuCount()
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


QString StatusReader::_cpu_temp_path = "/sys/class/thermal/thermal_zone0/temp";
QString StatusReader::_cpu_state_path = "/proc/stat";

QMutex StatusReader::_mutex(QMutex::NonRecursive);
QMap<QUuid, StatusReader*> StatusReader::_readers;
