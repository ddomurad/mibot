#ifndef MIBUTILS_GLOBAL_H
#define MIBUTILS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MIBUTILS_LIBRARY)
#  define MIBUTILSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MIBUTILSSHARED_EXPORT Q_DECL_IMPORT
#endif

#define LOG_DEBUG(MSG) mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::Debug, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_INFO(MSG) mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::Info, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_IMPORTANT(MSG) mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::ImportantInfo, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_WARNING(MSG) mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_ERROR(MSG) mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::Error, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_MESSAGE(MSG) mibot::LoggerManager::instance()->WriteMessage( MSG )


namespace mibot
{
    enum class LogLevel
    {
        Debug = 0,
        Info = 1,
        ImportantInfo = 2,
        Warning = 3,
        Error = 4
    };
}

#endif // LOGGER_GLOBAL_H
