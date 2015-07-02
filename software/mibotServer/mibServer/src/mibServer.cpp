#include <mibLogger.h>

#include <mibSettingsClient.h>
#include <mibSettingsDefs.h>

#include "inc/mibConnection.h"
#include "inc/mibServer.h"

#include <mibGPIO.h>

using namespace mibot;

Server::Server(QObject *parent):
    QObject(parent)
{}

Server::~Server()
{
    for(int i=0; i<_listeners.count(); i++)
        delete _listeners[i];

    if(_connectionHandler != nullptr)
        delete _connectionHandler;
}

Server *Server::BuildServer(QJsonObject &config, QObject * parent)
{
    Server * out_server = new Server(parent);
    out_server->_listenersThread = new QThread(out_server);
    out_server->_connectionHandler = new ConnectionHandler();
    out_server->_connectionHandler->moveToThread( out_server->_listenersThread );

    if(config["Ssl"].isNull())
    {
        LOG_ERROR("No ssl config defined.");
        return out_server;
    }

    QJsonObject sslConfig = config["Ssl"].toObject();
    if(!sslConfig["Cert"].isString() || !sslConfig["CaCerts"].isString())
    {
        LOG_ERROR("Invalid ssl config!");
        return out_server;
    }

    QStringList socketsList = SettingsClient::GetResourceList(SocketSetting::DefaultDir, 5000);

    if(socketsList.empty())
    {
        LOG_ERROR("Can't read sockets!");
        return out_server;
    }

    for(QString socketResName : socketsList)
    {
        QString socketResPath = SocketSetting::DefaultDir + socketResName;

        SocketSetting * socketObj = SettingsClient::CreateReource<SocketSetting>(socketResPath);
        if(!socketObj->Sync())
        {
            LOG_ERROR(QString("Can't read socket settings: %1").arg(socketResPath));
            continue;
        }

        LOG_IMPORTANT( QString("Adding listener (Name='%1', Port='%2', UseSSL=%3).").
                       arg(socketObj->alias->value, QString::number(socketObj->port->value), socketObj->useSsl->value ? "Yes" : "No") );

        Listener * list = new Listener( socketObj );

        if( !list->InitCertificates(sslConfig["CaCerts"].toString(), sslConfig["Cert"].toString()) )
        {
            LOG_ERROR( QString("Can't intialize certificates for socker: %1").arg(socketObj->alias->value));
            delete list;
        }else
        {
            connect( list, SIGNAL(NewConnection(Connection*)), out_server->_connectionHandler, SLOT(NewConnection(Connection*)));
            out_server->AddListener( list );
        }
    }

    return out_server;
}

void Server::AddListener(Listener *listener)
{
    listener->moveToThread( _listenersThread );

    connect(_listenersThread, SIGNAL(started()), listener, SLOT(StartListen()));
    connect(_listenersThread, SIGNAL(finished()), listener, SLOT(StopListen()));

    _listeners.append( listener );
}

void Server::StartServer()
{
    _listenersThread->start();
    emit ServerStarted();
}

void Server::StopServer()
{
    _listenersThread->quit();
    _listenersThread->wait();
    emit ServerStoped();
}



