#include "inc/mibLoggerSink.h"

using namespace mibot;

LoggerSink::LoggerSink(LogLevel level, LoggerFormater *formater) :
    _level(level),
    _formater(formater)
{}

LoggerSink::~LoggerSink()
{
    if(_formater != nullptr)
        delete _formater;
}

void LoggerSink::WriteLog(
        LogLevel level, QString file,
        QString function, qint32 line,
        QString message)
{
    this->Write( _formater->FormatMessage(level, file, function, line, message) );
}

LogLevel LoggerSink::GetLevel()
{
    return _level;
}
