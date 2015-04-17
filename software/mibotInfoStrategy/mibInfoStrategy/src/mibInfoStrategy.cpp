#include <mibLogger.h>
#include <mibGlobalAccess.h>

#include "inc/mibInfoStrategy.h"

using namespace mibot;
InfoStrategy::InfoStrategy(Connection *connection)
    : AbstractSocketStrategy(connection)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));
}

InfoStrategy::~InfoStrategy()
{

}

void InfoStrategy::onStrategyUpdate()
{
    fixIfJsonIsCorrupted();

    if(_json_protocol.ContainsValidJSONObject())
    {
        QJsonObject object = _json_protocol.GetPendingObject();
        fixIfJsonIsCorrupted();

        QJsonObject ret_obj = createRequest(object);
        if(!ret_obj.isEmpty())
        {
            QJsonDocument doc( ret_obj);
            _connection->TcpSocket->write( doc.toJson( QJsonDocument::Compact ) );
        }
    }
}

void InfoStrategy::processNewData(QByteArray data)
{
    _json_protocol.PushData(data);
}

bool InfoStrategy::init()
{
    _timer->start( 1000 );

    return true;
}

QJsonObject InfoStrategy::createRequest(QJsonObject &obj)
{
    QJsonObject ret_obj;
    QList<QString> reqest_list;

    if(obj["get"].isArray())
    {
        QJsonArray get_array = obj["get"].toArray();
        for(int i=0; i<get_array.count(); i++)
        {
            if(!get_array.at(i).isString()) continue;
            QString info_type = get_array.at(i).toString();

            if(reqest_list.contains(info_type)) continue;
            reqest_list.append( info_type );

            getInfoObject(ret_obj, info_type);
        }
    }

    return ret_obj;
}

void InfoStrategy::getInfoObject(QJsonObject &obj, QString request)
{
    if(request == "sockets")
    {
        obj.insert("sockets",QJsonValue(getSocketInfo()));
    }
}

QJsonArray InfoStrategy::getSocketInfo()
{
    int client_priviliges = _connection->UserObj()->PrivilegesObj->Level();

    ResourcesSet<SocketRes> * socket_set =  GlobalAccess::AllEnabledSockets();
    if(socket_set == nullptr) return QJsonArray();
    QJsonArray arr;
    for(int i=0;i<socket_set->Count(); i++)
    {
        SocketRes * socket = socket_set->At(i);

        if(socket->PrivilegesObj == nullptr)
            continue;

        if(socket->PrivilegesObj->Level() > client_priviliges)
            continue;

        QJsonObject json_socket;
        json_socket.insert("type", QJsonValue(socket->Strategy()));
        json_socket.insert("privileges", QJsonValue(socket->PrivilegesObj->Level()));
        json_socket.insert("port", QJsonValue(socket->Port()));
        json_socket.insert("ssl", QJsonValue(socket->UseSsl()));
        arr.append( QJsonValue(json_socket) );

    }

    delete socket_set;
    return arr;
}

void InfoStrategy::fixIfJsonIsCorrupted()
{
    if(_json_protocol.IsDataCorupted())
    {
        LOG_WARNING("StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        _json_protocol.RemoveCoruptedData();
    }
}

AbstractSocketStrategy *createStrategy(Connection *connection)
{
    return new InfoStrategy( connection );
}
