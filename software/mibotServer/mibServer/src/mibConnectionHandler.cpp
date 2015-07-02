#include "inc/mibConnectionHandler.h"

#include <mibLogger.h>
#include <QHostAddress>

using namespace mibot;

ConnectionHandler::ConnectionHandler(QObject *parent) : QObject(parent)
{}

ConnectionHandler::~ConnectionHandler()
{
    for( Connection * c : _connections)
        delete c;
}

void ConnectionHandler::NewConnection(Connection * connection)
{
    LOG_IMPORTANT(connection->Dump(true));

    if(connection->Status == Connection::Success)
    {
        if(connection->UseSsl)
        {
            if(connection->UserObj == nullptr)
            {
                pushConnectionAudit( connection, "ConnctionFailure" , "Unknown user");
                connection->TcpSocket->disconnectFromHost();

                delete connection;
                return;
            }

            PrivilegeSetting * userPrivObj = SettingsClient::CreateReource<PrivilegeSetting>(connection->UserObj->privileges->value);
            PrivilegeSetting * socketPrivObj = SettingsClient::CreateReource<PrivilegeSetting>(connection->SocketObj->privileges->value);

            if(!userPrivObj->Sync())
            {
                LOG_ERROR( QString("Can't get users '%1' privilige settings: '%2'")
                           .arg(connection->UserObj->alias->value)
                           .arg(connection->UserObj->privileges->value));

                pushConnectionAudit( connection, "ConnctionFailure" , "Unknown user access level");

                connection->TcpSocket->disconnectFromHost();

                userPrivObj->Release();
                delete connection;
                return;
            }

            if(!socketPrivObj->Sync())
            {
                LOG_ERROR( QString("Can't get socket '%1' privilige settings: '%2'")
                           .arg(connection->SocketObj->alias->value)
                           .arg(connection->UserObj->privileges->value));

                pushConnectionAudit( connection, "ConnctionFailure" , "Unknown socket access level");

                connection->TcpSocket->disconnectFromHost();

                socketPrivObj->Release();
                userPrivObj->Release();

                delete connection;
                return;
            }

            if( userPrivObj->level->value < socketPrivObj->level->value)
            {
                pushConnectionAudit( connection, "ConnctionFailure" , "Access level too low");

                connection->TcpSocket->disconnectFromHost();

                socketPrivObj->Release();
                userPrivObj->Release();

                delete connection;
                return;
            }

            socketPrivObj->Release();
            userPrivObj->Release();
        }

        if(!AbstractSocketStrategy::ApplyStrategy(connection))
        {
            pushConnectionAudit( connection, "ConnctionFailure" , "Can't apply stategy");

            connection->TcpSocket->disconnectFromHost();
            delete connection;
            return;
        }

        pushConnectionAudit( connection, "Connected" , QString());
        addConnection(connection);
    }
    else
    {
        pushConnectionAudit( connection, "ConnctionFailure", connection->ErrorStrnig );
        connection->TcpSocket->close();
        delete connection;
        return;
    }
}

void ConnectionHandler::onDisconnected()
{
    Connection * connection = connectionFromSocket( (QTcpSocket *)sender());
    LOG_IMPORTANT(connection->Dump(true));

    pushConnectionAudit( connection, "Disconnected", QString() );

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

void ConnectionHandler::pushConnectionAudit(Connection *connection, QString entry_type, QString entry_desc)
{
    QString addr = "unknown";
    QString user = "unknown";
    QString socket = "unknown";
    QString strategy = "unknown";

    if(connection->TcpSocket != nullptr)
        addr = connection->TcpSocket->peerAddress().toString();

    if(connection->UserObj != nullptr)
        user = connection->UserObj->alias->value;

    if(connection->SocketObj != nullptr)
        socket = connection->SocketObj->alias->value;

    if(connection->SocketObj != nullptr)
        strategy = connection->SocketObj->strategy->value;


    LOG_IMPORTANT( QString("%1 (%2) from '%3@%4' on socket '%5' with strategy '%6'")
                   .arg(entry_type, entry_desc,user,addr, socket, strategy));
}

