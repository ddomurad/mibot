#include <QDebug>
#include <QCoreApplication>
#include <mibSettingsClient.h>
#include <mibSettingsObject.h>
#include <mibLogger.h>
#include <mibLoggerStandardSinks.h>

#include <mibArduinoSensorNode.h>

#include <pthread.h>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    mibot::LoggerManager::instance()->AddSink(
                new mibot::LoggerConsoleSink(
                    mibot::LogLevel::Debug,
                    new mibot::LoggerSimpleConsoleFormater()));

    mibot::LoggerManager::instance()->AddSink(
                new mibot::LoggerFileSink("./log.txt",
                    mibot::LogLevel::Warning,
                    new mibot::LoggerSimpleStringFormater()));

    mibot::LoggerManager::instance()->WriteLog(mibot::LogLevel::Info, "main.cpp", "main", 43, "Test message");
    LOG_WARNING("warning message text");
    return app.exec();
}
