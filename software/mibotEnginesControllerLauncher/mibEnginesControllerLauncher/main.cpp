#include <QCoreApplication>
#include <QtNetwork>

#include <mibLogger.h>
#include <mibGlobalAccess.h>
#include <mibStandardLoggerBuilder.h>
#include <mibEnginesController.h>
#include <mibControllerStatus.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile file("/usr/local/etc/mi_bot/mibot_engines_ctrl.config");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Can't load configuration file.";
        exit(1);
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson( file.readAll() );
    file.close();

    QJsonObject root_obj = jsonDoc.object();

    if(root_obj["Logger"].isNull())
    {
        qDebug() << "No logger configs.";
        exit(1);
    }

    if(root_obj["Access"].isNull())
    {
        qDebug() << "No Access configs.";
        exit(1);
    }

    mibot::StandardLoggerBuilder buildier;
    QJsonObject loggerObject = root_obj["Logger"].toObject();
    buildier.BuildLogger( loggerObject  );

    DEFLOG_INFO("Loading GlobalAccess");

    QJsonObject access_obj = root_obj["Access"].toObject();
    mibot::GlobalAccess::Init( access_obj );

    mibot::EnginesController * controller = new mibot::EnginesController(&a);
    controller->Init();

    return a.exec();
}
