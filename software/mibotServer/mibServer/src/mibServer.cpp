#include <mibLogger.h>
#include <mibGlobalAccess.h>
#include "inc/mibConnection.h"
#include "inc/mibServer.h"

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

    if(config["Listeners"].isNull())
    {
        DEFLOG_ERROR("No listeners defined.");
        return out_server;
    }

    QJsonArray listeners = config["Listeners"].toArray();
    for(int i=0; i<listeners.count(); i++)
    {
        QJsonObject listener = listeners.at(i).toObject();
        if( listener["Socket"].isNull())
        {
            DEFLOG_ERROR("Id is not defined for listener.");
            continue;
        }

        QString sockId = listener["Socket"].toString();
        SocketRes * sockRes = GlobalAccess::Socket( sockId );

        if(sockRes == nullptr)
        {
            DEFLOG_ERROR("Socket with given id does not exit: " + sockId );
            continue;
        }

        if(sockRes->PrivilegesObj == nullptr)
        {
            DEFLOG_ERROR("Socket privilage resolving error");
            delete sockRes;
            continue;
        }

        bool isSsl = !listener["Ssl"].isNull();

        DEFLOG_IMPORTANT( QString("Adding listener (Name='%1', Port='%2', UseSSL=%3).").
                          arg(sockRes->Alias(), QString::number(sockRes->Port()), isSsl ? "Yes" : "No") );

        Listener * list = new Listener( sockRes , isSsl);
        if(isSsl)
        {
            QJsonObject sslObject = listener["Ssl"].toObject();
            if(sslObject["LocalCertificateFile"].isNull() ||
               sslObject["LocalPrivateKeyFile"].isNull() ||
               sslObject["CaCertificates"].isNull())
            {
                DEFLOG_ERROR("Listeners Ssl definition is invalid.");
            }else
            {
                QString locCrt = sslObject["LocalCertificateFile"].toString();
                QString locKey = sslObject["LocalPrivateKeyFile"].toString();
                QString caCrt = sslObject["CaCertificates"].toString();

                list->SetCertificates( locCrt, locKey, caCrt );

                DEFLOG_DEBUG( QString("Setup ssl (LocalCertificateFile='%1', LocalPrivateKeyFile='%2', CaCertificates='%3')")
                              .arg(locCrt, locKey, caCrt));
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



