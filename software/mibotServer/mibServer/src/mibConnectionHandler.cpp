#include "inc/mibConnectionHandler.h"
#include <mibLogger.h>
#include <QHostAddress>

using namespace mibot;

ConnectionHandler::ConnectionHandler(QObject *parent) : QObject(parent)
{}

ConnectionHandler::~ConnectionHandler()
{}

void ConnectionHandler::NewConnection(Connection * connection)
{
    LOG_IMPORTANT(connection->Dump(true));

    if(connection->Status == Connection::Success)
    {
        if(connection->UseSsl)
        {
            if( connection->UserObj()->PrivilegesObj->Level() <
                    connection->SocketObj()->PrivilegesObj->Level())
            {
                pushAndReleaseAudit( connection, "ConnctionFailure" , "Access level too low");

                connection->TcpSocket->disconnectFromHost();
                delete connection;
                return;
            }
        }

        if(!AbstractSocketStrategy::ApplyStrategy(connection))
        {
            pushAndReleaseAudit( connection, "ConnctionFailure" , "Can't apply stategy");

            connection->TcpSocket->disconnectFromHost();
            delete connection;
            return;
        }

        pushAndReleaseAudit( connection, "Connected" , QString());
        addConnection(connection);
    }
    else
    {
        pushAndReleaseAudit( connection, "ConnctionFailure", connection->ErrorStrnig );
        connection->TcpSocket->close();
        delete connection;
        return;
    }
}

void ConnectionHandler::onDisconnected()
{
    Connection * connection = connectionFromSocket( (QTcpSocket *)sender());
    LOG_IMPORTANT(connection->Dump(true));

    pushAndReleaseAudit( connection, "Disconnected", QString() );

    removeConnection(connection);
}


Connection *ConnectionHandler::connectionFromSocket(QTcpSocket *socket)
{
    for(int i=0; _connections.length(); i++)
    {
        if(_connections[i]->TcpSocket == socket) return _connections[i];
    }

    return nullptr;
}

void ConnectionHandler::addConnection(Connection *connection)
{
    _connections.append( connection );

    connect( connection->TcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()) );
    connect( connection->TcpSocket, SIGNAL(readyRead()), connection->Strategy, SLOT(ProcessData()) );
}

void ConnectionHandler::removeConnection(Connection *connection)
{
    _connections.removeOne( connection );
    delete connection;
}

void ConnectionHandler::pushAndReleaseAudit(Connection *connection, QString entry_type, QString entry_desc)
{
    ConnectionAuditRes * audit = new ConnectionAuditRes;
    if(connection->TcpSocket != nullptr)
    {
        audit->SetFromAddr( connection->TcpSocket->peerAddress().toString() );
    }

    audit->SetConnectionId( connection->Id );
    audit->SetUser( connection->User );
    audit->SeteSocket( connection->Socket );
    audit->SetEntryTime( QDateTime::currentDateTime() );

    audit->SetEntryType( entry_type );
    audit->SetEntryDetails( entry_desc );
    GlobalAccess::PushConnectionAudit(audit);
    delete audit;
}

