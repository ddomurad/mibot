#include <QCoreApplication>
#include <qobject.h>
#include <QFile>
#include <QDebug>
#include <cstdio>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>

#include <mibServer.h>

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

        //_timer->start(1000);
    }


private:
    mibot::Server * _server;
    QTimer * _timer;
};

int main(int argc, char *argv[])
{
    TestApp a(argc,argv);

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


    mibot::StandardLoggerBuilder buildier;
    QJsonObject loggerObject = root_obj["Logger"].toObject();
    buildier.BuildLogger( loggerObject  );

    DEFLOG_INFO("Server start ...");

    QJsonObject serverObject = root_obj["Server"].toObject();
    a.RunServer( mibot::Server::BuildServer( serverObject, &a ) );

    a.exec();
}
