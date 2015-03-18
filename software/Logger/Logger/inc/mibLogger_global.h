#ifndef LOGGER_GLOBAL_H
#define LOGGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGGER_LIBRARY)
#  define LOGGERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGGERSHARED_EXPORT Q_DECL_IMPORT
#endif

#define LOGGER(__NAME__) (*mibot::LoggerManager::instance()->GetChannel(__NAME__))
#define ADD_LOGGER(__NAME__, __CHANNEL__) (mibot::LoggerManager::instance()->AddChannel(__NAME__,__CHANNEL__))
#define CREATE_LOGGER(__NAME__, __LEVEL__) (mibot::LoggerManager::instance()->AddChannel(__NAME__,new mibot::LoggerChannel(__LEVEL__)))
#define SET_DEF_LOGGER(__NAME__) (mibot::LoggerManager::instance()->SelectDefaultChannel(__NAME__))
#define DEF_LOGGER (*mibot::LoggerManager::instance()->GetDefaultChannel())

#define LOG_DEBUG(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteLog(mibot::LogLevel::Debug, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_INFO(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteLog(mibot::LogLevel::Info, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_IMPORTANT(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteLog(mibot::LogLevel::ImportantInfo, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_WARNING(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteLog(mibot::LogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_ERROR(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteLog(mibot::LogLevel::Error, __FILE__, __FUNCTION__, __LINE__, MSG)
#define LOG_MESSAGE(__NAME__,MSG) (*mibot::LoggerManager::instance()->GetChannel(__NAME__)).WriteMessage( MSG )

#define DEFLOG_DEBUG(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteLog(mibot::LogLevel::Debug, __FILE__, __FUNCTION__, __LINE__, MSG)
#define DEFLOG_INFO(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteLog(mibot::LogLevel::Info, __FILE__, __FUNCTION__, __LINE__, MSG)
#define DEFLOG_IMPORTANT(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteLog(mibot::LogLevel::ImportantInfo, __FILE__, __FUNCTION__, __LINE__, MSG)
#define DEFLOG_WARNING(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteLog(mibot::LogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, MSG)
#define DEFLOG_ERROR(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteLog(mibot::LogLevel::Error, __FILE__, __FUNCTION__, __LINE__, MSG)
#define DEFLOG_MESSAGE(MSG) (*mibot::LoggerManager::instance()->GetDefaultChannel()).WriteMessage( MSG )

#define WRITE_DEBUG(MSG) WriteLog(mibot::LogLevel::Debug, __FILE__, __FUNCTION__, __LINE__, MSG)
#define WRITE_INFO(MSG) WriteLog(mibot::LogLevel::Info, __FILE__, __FUNCTION__, __LINE__, MSG)
#define WRITE_IMPORTANT(MSG) WriteLog(mibot::LogLevel::ImportantInfo, __FILE__, __FUNCTION__, __LINE__, MSG)
#define WRITE_WARNING(MSG) WriteLog(mibot::LogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, MSG)
#define WRITE_ERROR(MSG) WriteLog(mibot::LogLevel::Error, __FILE__, __FUNCTION__, __LINE__, MSG)
#define WRITE_MESSAGE(MSG) WriteMessage( MSG )


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
