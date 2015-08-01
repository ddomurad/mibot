#include <QDebug>
#include <QCoreApplication>
#include <mibSettingsClient.h>
#include <mibSettingsObject.h>
#include <mibLogger.h>
#include <mibLoggerStandardSinks.h>

#include <mibArduinoSensorNode.h>

#include <pthread.h>


void TestFunction()
{



}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    mibot::LoggerManager::instance()->AddSink(
                new mibot::LoggerConsoleSink(
                mibot::LogLevel::Debug,
                new mibot::LoggerSimpleConsoleFormater()));

    LOG_INFO("Test log ...");
    QJsonObject settings;
    settings.insert("cert",QJsonValue("/home/work/Projects/praca_mgr/mibot/software/tools/SettingsManager/debug/certs/manager"));
    settings.insert("caCerts",QJsonValue("/home/work/Projects/praca_mgr/mibot/software/tools/SettingsManager/debug/certs/trusted.pem"));
    settings.insert("addr",QJsonValue("localhost"));
    settings.insert("port",QJsonValue(20400));
    settings.insert("peerName",QJsonValue("SettingsServer"));

    if(!mibot::SettingsClient::StartClient( settings , 10 * 60 * 1000))
    {
        LOG_ERROR("Can't start settigns cloient !");
        return 1;
    }

    TestFunction();

    QThread::sleep(1);
    mibot::SettingsClient::StopClient();

    return app.exec();
}
