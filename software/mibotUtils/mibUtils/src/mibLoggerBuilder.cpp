#include "inc/mibLoggerBuilder.h"

using namespace mibot;




void LoggerBuilder::LogProcess(LoggerBuilder::LOG_TYPE type, QString message)
{
    LogEntry entry {type, message};
    _log.append(entry);
}

QList<LoggerBuilder::LogEntry> LoggerBuilder::GetBuildLog(bool onlyErrors)
{
    if(onlyErrors)
    {
        QList<LogEntry> errorList;
        for(auto entry : _log)
        {
            if(entry.type == LOG_TYPE::ERROR)
                errorList.append(entry);
        }
        return errorList;
    }

    return _log;
}

bool LoggerBuilder::AreErrors()
{
    for(auto log : _log)
        if(log.type == LOG_TYPE::ERROR) return true;

    return false;
}
