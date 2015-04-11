#include <mibLogger.h>
#include <mibGlobalAccess.h>
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

    if(config["Sockets"].isNull())
    {
        LOG_ERROR("No listeners defined.");
        return out_server;
    }

    QJsonArray listeners = config["Sockets"].toArray();
    for(int i=0; i<listeners.count(); i++)
    {
        QJsonObject listener = listeners.at(i).toObject();
        if( listener["Socket"].isNull())
        {
            LOG_ERROR("Id is not defined for listener.");
            continue;
        }

        QString sockId = listener["Socket"].toString();
        SocketRes * sockRes = GlobalAccess::Socket( sockId );

        if(sockRes == nullptr)
        {
            LOG_ERROR("Socket with given id does not exit: " + sockId );
            delete sockRes;
            continue;
        }

        if(!sockRes->IsEnabled())
        {
            LOG_WARNING("Socket with given id is disabled: " + sockId );
            delete sockRes;
            continue;
        }

        if(sockRes->PrivilegesObj == nullptr)
        {
            LOG_ERROR("Socket privilage resolving error: " + sockId);
            delete sockRes;
            continue;
        }


        LOG_IMPORTANT( QString("Adding listener (Name='%1', Port='%2', UseSSL=%3).").
                          arg(sockRes->Alias(), QString::number(sockRes->Port()), sockRes->UseSsl() ? "Yes" : "No") );

        Listener * list = new Listener( sockRes );

        auto sslSettings = listener["Ssl"];
        if(sslSettings.isNull())
        {
            LOG_WARNING(QString("No ssl certificates setings specified. (%1)")
                           .arg( sockRes->Alias() ));

            if(sockRes->UseSsl())
            {
                LOG_ERROR(QString("This listener (%1) need to use ssl, but no setings are spesified.")
                             .arg( sockRes->Alias() ));

                delete list;
                continue;
            }
        }else
        {
            QString crtDir = sslSettings.toObject()["CrtDir"].toString();
            QString crtName = sslSettings.toObject()["Crt"].toString();

            LOG_DEBUG( QString("Setup ssl ( CrtDir='%1', Crt='%2' )")
                          .arg( crtDir, crtName ));

            if(!list->InitCertificates( crtDir, crtName ))
            {
                LOG_ERROR("Certificates initialization error.");
            }

        }

        connect( list, SIGNAL(NewConnection(Connection*)), out_server->_connectionHandler, SLOT(NewConnection(Connection*)));

        out_server->AddListener( list );
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



