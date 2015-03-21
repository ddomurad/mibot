#include <QCoreApplication>
#include <qobject.h>
#include <QFile>
#include <QDebug>
#include <cstdio>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>

#include <mibServer.h>
#include <mibAbstractResource.h>
#include <mibResourceWrapper.h>
#include <mibSqlRepository.h>
#include <mibGlobalAccess.h>
#include <mibConnectionHandler.h>

#define TRACE(...) qDebug() << #__VA_ARGS__ << __VA_ARGS__


class TestApp : public QCoreApplication
{
public:
    TestApp(int &argc,char * argv[]) :
        QCoreApplication(argc,argv)
    {}


    void RunServer(mibot::Server * server)
    {
        _server = server;
        _timer = new QTimer(this);

        connect(_timer, SIGNAL(timeout()), _server,SLOT(StopServer()));
        connect( server, SIGNAL(ServerStoped()), this,SLOT(quit()));

        emit _server->StartServer();

        //_timer->start(10000);
    }


private:
    mibot::Server * _server;
    QTimer * _timer;
};

void testFnc()
{
    QVariant var = QVariant (QVariant::Uuid);
    var.setValue( QUuid::createUuid() );
    QString str = var.toString();
    qDebug() << str;
     str = str.mid(1,36);

    qDebug() << str;
}

int main(int argc, char *argv[])
{
    TestApp a(argc,argv);

//    testFnc(); return 0;

    QFile file("./app.config");
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

    if(root_obj["Server"].isNull())
    {
        qDebug() << "No server configs.";
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

    DEFLOG_INFO("Loading Server");
    QJsonObject server_obj = root_obj["Server"].toObject();
    mibot::Server * server =  mibot::Server::BuildServer( server_obj,  &a);

    a.RunServer(  server );

    a.exec();
}
