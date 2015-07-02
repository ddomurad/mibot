#include <QDebug>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QFile>
#include <mibSettingsServer.h>
#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Loading Settings";

    QFile settingsFile("./settings");
    if(!settingsFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't load settings file";
        return 1;
    }

    QJsonDocument doc = QJsonDocument::fromJson(settingsFile.readAll());
    QJsonObject settigns = doc.object();
    settingsFile.close();

    qDebug() << "Initialize Logger";
    mibot::StandardLoggerBuilder buildier;
    QJsonObject loggerObject = settigns["Logger"].toObject();
    buildier.BuildLogger( loggerObject  );
    if(buildier.AreErrors())
    {
        qDebug() << "FATAL ERROR: Can't build logger.";
        for(auto log : buildier.GetBuildLog(true))
            qDebug() << "ERROR: " << log.message;

        return 2;
    }

    LOG_IMPORTANT("Starting Server.");

    mibot::SettingsServer * server = new mibot::SettingsServer();
    QJsonObject serverSettings = settigns["Server"].toObject();
    if(!server->startServer(serverSettings))
        return 3;

    return a.exec();
}
