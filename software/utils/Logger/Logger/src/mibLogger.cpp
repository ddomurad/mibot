#include "inc/mibLogger.h"
#include <cstdio>

using namespace mibot;

LoggerManager::LoggerManager()
{}

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
    auto channel = instance()->_channels.value(name, nullptr);
    if(channel == nullptr)
        printf(_given_channel_dont_exists.toStdString().c_str(),channel);
    return channel;
}

LoggerChannel *LoggerManager::GetDefaultChannel()
{
    return _default_channel;
}

void LoggerManager::AddChannel(QString name, LoggerChannel *channel)
{
    instance()->_channels.insert(name, channel);
    if(_default_channel == nullptr)
       _default_channel = channel;
}

void LoggerManager::SelectDefaultChannel(QString name)
{
    auto channel = GetChannel(name);
    if(channel == nullptr) return;
    _default_channel = channel;
}


LoggerChannel * LoggerManager::_default_channel = nullptr;
QString LoggerManager::_given_channel_dont_exists = "!!! FATAL_LOG_ERROR: Can't find channel: '%s'";
