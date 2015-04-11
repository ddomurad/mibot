#include "inc/mibStatusReader.h"
#include <mibLogger.h>

#include <stdio.h>
#include <stdlib.h>
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
        double val = readSystemStateValue("/sys/class/thermal/thermal_zone0/temp").toDouble();
        _readings.insert( CpuTemperature , QVariant(val) );
    }
}

QString StatusReader::readSystemStateValue(QString path)
{
    int fd;
    char buf[__BUF_SIZE];
    memset( buf, 0, __BUF_SIZE );

    if ((fd = open(path.toStdString().c_str(), O_RDONLY)) == -1)
    {
        return "";
    }

    if ((read(fd, buf, __BUF_SIZE)) == -1)
    {
        close(fd);
        return "";
    }

    close(fd);

    return QString(buf);
}

QMutex StatusReader::_mutex(QMutex::NonRecursive);
QMap<QUuid, StatusReader*> StatusReader::_readers;
