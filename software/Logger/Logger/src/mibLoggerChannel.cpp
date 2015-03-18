#include "inc/mibLoggerChannel.h"

using namespace mibot;

LoggerChannel::LoggerChannel(LogLevel level) :
    _level(level)
{}

LoggerChannel::LoggerChannel(LogLevel level, std::initializer_list<LoggerOutput *> list)
    : LoggerChannel(level)
{
    for( auto out : list)
        _outputs.append( out );
}

LoggerChannel::~LoggerChannel()
{
    for(int i=0; i< _outputs.length(); i++)
        delete _outputs[i];
}

void LoggerChannel::Append(std::initializer_list<LoggerOutput *> list)
{
    for( auto out : list)
        _outputs.append( out );
}

void LoggerChannel::operator +=(LoggerOutput *out)
{
    this->_outputs.append(out);
}

void LoggerChannel::operator +=(std::initializer_list<LoggerOutput *> list)
{
    Append(list);
}

void LoggerChannel::WriteLog(LogLevel level, QString file, QString function,
                             uint line, QString message)
{
    if(level < _level) return;
    for(auto output : _outputs)
        output->WriteLog(level, file, function, line, message);
}

void LoggerChannel::WriteMessage(QString msg)
{
    for(auto output : _outputs)
        output->Write(msg);
}
