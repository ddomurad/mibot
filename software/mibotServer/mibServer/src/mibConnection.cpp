#include "inc/mibConnection.h"
#include <QHostAddress>

using namespace mibot;

Connection::Connection() :
    Strategy(nullptr),
    UserObj(nullptr),
    SocketObj(nullptr)
{}

Connection::~Connection()
{
    if( Strategy != nullptr)
        delete Strategy;

    if(UserObj != nullptr)
        UserObj->Release();

    if(SocketObj != nullptr)
       SocketObj->Release();
}


QString Connection::Dump(bool deep)
{
    if(deep)
    {

        QString socket_dump;
        if(TcpSocket == nullptr)
            socket_dump = "null";
        else
            socket_dump = QString("(From:%1:%2)")
                    .arg(TcpSocket->peerAddress().toString())
                    .arg(TcpSocket->peerPort());

        return QString("{ Socket:{ %1 } }, User:{ %2  } , Socket:{ %3 }, Other:{ (Status:%4), (Ssl:%5) } }")
                .arg( SocketObj == nullptr ? "read_error" : SocketObj->Dump() )
                .arg( UserObj == nullptr ? "read_error" : UserObj->Dump() )
                .arg( socket_dump )
                .arg( Status == Connection::Success ? "Success" : "Failed ("+ErrorStrnig+")")
                .arg( UseSsl );
    }
    else
        return QString( "{User: %1, Socket: %2, Status: %3, Ssl: %4}" )
                .arg( UserObj == nullptr ? "read_error" : UserObj->alias->value )
                .arg( SocketObj == nullptr ? "read_error" : SocketObj->alias->value)
                .arg( Status == Connection::Success ? "Success" : "Failed" )
                .arg( UseSsl );
}
