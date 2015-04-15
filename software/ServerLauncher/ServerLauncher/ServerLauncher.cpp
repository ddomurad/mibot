#include <signal.h>
#include "ServerLauncher.h"
#include <QDebug>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibGlobalAccess.h>

ServerLauncher::ServerLauncher(QCoreApplication *parent) : QObject(parent)
{
    _server = nullptr;
    connect(this,SIGNAL(Closed()),parent, SLOT(quit()) );
}

ServerLauncher::~ServerLauncher()
{

}

bool ServerLauncher::Init(QStringList args)
{
    if(_launcher != nullptr) return false;
    _launcher = this;

    if( signal(SIGTERM, ServerLauncher::_signalHandler) == SIG_ERR)
    {
        qDebug() << "Can't catch SIGTERM.";
        return false;
    }

    if( signal(SIGHUP, ServerLauncher::_signalHandler) == SIG_ERR)
    {
        qDebug() << "Can't catch SIGHUP.";
        return false;
    }

    if( signal(SIGALRM, ServerLauncher::_signalHandler) == SIG_ERR)
    {
        qDebug() << "Can't catch SIGALRM.";
        return false;
    }

    if( signal(SIGINT, ServerLauncher::_signalHandler) == SIG_ERR)
    {
        qDebug() << "Can't catch SIGINT.";
        return false;
    }

    QString config_file_patg = "/usr/local/etc/mi_bot/mibot_server.config";
    for(int i = 0; i < args.count(); i++)
    {
        if(args[i] == "-c" && args.count() > (i+1))
            config_file_patg = args[++i];
    }

    qDebug() << "Loaging config file.";

    QFile file(config_file_patg);
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

    qDebug() << "Initialize Global Access";
    QJsonObject access_obj = root_obj["Access"].toObject();
    mibot::GlobalAccess::Init( access_obj );

    qDebug() << "Initialize Logger";
    mibot::StandardLoggerBuilder buildier;
    QJsonObject loggerObject = root_obj["Logger"].toObject();
    buildier.BuildLogger( loggerObject  );

    LOG_IMPORTANT("Mibot Server Starting.");

    LOG_IMPORTANT("Loading Server.");
    QJsonObject server_obj = root_obj["Server"].toObject();
    _server =  mibot::Server::BuildServer( server_obj,  this);

    connect( _server, SIGNAL(ServerStarted()), this, SLOT(onServerStarted()) );
    connect( _server, SIGNAL(ServerStoped()), this, SLOT(onServerStoped()) );

    qDebug() << "Server starting";
    _server->StartServer();

    return true;
}

void ServerLauncher::Close()
{
    //mibot::LoggerManager::instance()->Drop();
    if(_server != nullptr)
    {
        _server->StopServer();
    }
    //emit Closed();
}

void ServerLauncher::onServerStarted()
{
    qDebug() << "Server started.";
}

void ServerLauncher::onServerStoped()
{
    qDebug() << "Server stoped.";
    delete _server;
    emit Closed();
}

void ServerLauncher::signalHandler(int sig)
{
    qDebug() << "Signal retrived: " << sig;

    if(sig == SIGTERM  || sig == SIGHUP  ||
        sig == SIGINT  || sig == SIGALRM)
        Close();
}

void ServerLauncher::_signalHandler(int sig)
{
    _launcher->signalHandler( sig );
}


ServerLauncher * ServerLauncher::_launcher = nullptr;
