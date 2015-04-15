#include "inc/mibLogger.h"
#include <cstdio>

using namespace mibot;

LoggerManager::LoggerManager()
{}

LoggerManager::~LoggerManager()
{
    for( auto sink : _sinks)
        delete sink;
}

LoggerManager *LoggerManager::instance()
{
 static LoggerManager manager;
 return &manager;
}

void LoggerManager::WriteLog(LogLevel level, QString file, QString function, uint line, QString message)
{
    for( LoggerSink *sink : _sinks)
    {
        if( level >= sink->GetLevel() )
            sink->WriteLog(level,file,function,line,message);
    }
}

void LoggerManager::WriteMessage(QString msg)
{
    for( LoggerSink *sink : _sinks)
        sink->Write( msg );
}

void LoggerManager::AddSink(LoggerSink *sink)
{
    _sinks.append( sink );
}

void LoggerManager::Drop()
{
    for( auto sink : _sinks)
        delete sink;

    _sinks.clear();
}
