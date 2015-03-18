#include "inc/mibLoggerOutput.h"

using namespace mibot;

LoggerOutput::LoggerOutput(LoggerFormater *formater) :
    _formater(formater)
{}

LoggerOutput::~LoggerOutput()
{
    delete _formater;
}

void LoggerOutput::WriteLog(
        LogLevel level, QString file,
        QString function, qint32 line,
        QString message)
{
    this->Write( _formater->FormatMessage(level, file, function, line, message) );
}
