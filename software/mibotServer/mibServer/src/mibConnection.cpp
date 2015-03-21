#include "inc/mibConnection.h"
#include <QHostAddress>

using namespace mibot;

Connection::Connection() :
    Strategy(nullptr),
    _user(nullptr),
    _socket(nullptr)
{}

Connection::~Connection()
{
    if( Strategy != nullptr)
        delete Strategy;

    if(_user != nullptr)
        delete _user;

    if(_socket != nullptr)
        delete _socket;
}

UserRes *Connection::UserObj()
{
    if(_user == nullptr)
    {
        _user = GlobalAccess::User( User );
    }

    return _user;
}

SocketRes *Connection::SocketObj()
{
    if(_socket == nullptr)
    {
        _socket = GlobalAccess::Socket( Socket );
    }

    return _socket;
}

QString Connection::Dump(bool deep)
{
    if(deep)
    {
        SocketRes *sock = SocketObj();
        UserRes   *user = UserObj();


        QString socket_dump;
        if(TcpSocket == nullptr)
            socket_dump = "null";
        else
            socket_dump = QString("(From:%1:%2)")
                    .arg(TcpSocket->peerAddress().toString())
                    .arg(TcpSocket->peerPort());

        return QString("{ Socket:{ %1 Privileges:{ %5 } }, User:{ %2 Privileges:{ %6 } } , Socket:{ %3 }, Other:{ (Status:%4), (Ssl:%7) } }")
                .arg( sock == nullptr ? "read_error" : sock->Dump() )
                .arg( user == nullptr ? "read_error" : user->Dump() )
                .arg( socket_dump )
                .arg( Status == Connection::Success ? "Success" : "Failed ("+ErrorStrnig+")")
                .arg( sock == nullptr ? "read_error" : ( sock->PrivilegesObj == nullptr ? "read_error" : sock->PrivilegesObj->Dump()) )
                .arg( user == nullptr ? "read_error" : ( user->PrivilegesObj == nullptr ? "read_error" : user->PrivilegesObj->Dump()) )
                .arg( UseSsl );
    }
    else
        return QString( "{User: %1, Socket: %2, Status: %3, Ssl: %4}" )
                .arg( User.toString(), Socket.toString() )
                .arg( Status == Connection::Success ? "Success" : "Failed" )
                .arg( UseSsl );
}
