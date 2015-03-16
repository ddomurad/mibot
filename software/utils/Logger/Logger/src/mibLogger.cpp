#include "inc/mibLogger.h"
#include <cstdio>

using namespace mibot;

LoggerManager::LoggerManager():
    _default_channel(nullptr)
{
    _channels = QMap<QString, LoggerChannel*>();
}

LoggerManager::~LoggerManager()
{
    QStringList keys = _channels.keys();
    for( auto key : keys)
        delete _channels.value(key);
}

LoggerManager *LoggerManager::instance()
{
 static LoggerManager manager;
 return &manager;
}

LoggerChannel *LoggerManager::GetChannel(QString name)
{
    return _channels.value(name, nullptr);
}

LoggerChannel *LoggerManager::GetDefaultChannel()
{
    return _default_channel;
}

void LoggerManager::AddChannel(QString name, LoggerChannel *channel)
{
    _channels.insert(name, channel);
    if(_default_channel == nullptr)
       _default_channel = channel;
}

void LoggerManager::SelectDefaultChannel(QString name)
{
    auto channel = GetChannel(name);
    if(channel == nullptr) return;
    _default_channel = channel;
}

