#include <mibLogger.h>
#include <mibSettingsClient.h>
#include <mibSettingsDefs.h>

#include "inc/mibInfoStrategy.h"

using namespace mibot;
InfoStrategy::InfoStrategy(Connection *connection)
    : AbstractSocketStrategy(connection)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onStrategyUpdate()));
}

InfoStrategy::~InfoStrategy()
{}

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
    PrivilegeSetting * userPrivObj = getPrivileges( _connection->UserObj->privileges->value );

    if(userPrivObj == nullptr) return QJsonArray();
    int client_priviliges = userPrivObj->level->value;

    userPrivObj->Release();

    QStringList socketsList = SettingsClient::GetResourceList(SocketSetting::DefaultDir);

    if(socketsList.empty())
    {
        LOG_ERROR("Can't get socket list");
        return QJsonArray();
    }

    QJsonArray arr;
    for(QString socketResName : socketsList)
    {
        QString socketResPath = SocketSetting::DefaultDir + socketResName;

        SocketSetting * socketObj = SettingsClient::CreateReource<SocketSetting>(socketResPath);
        if(!socketObj->Sync(SC_OP_DEF_TIME, true))
        {
            LOG_ERROR("Can't get socket settings");
            socketObj->Release();
            continue;
        }

        PrivilegeSetting * socketPriv = getPrivileges( socketObj->privileges->value );

        if(socketPriv == nullptr)
        {
            socketObj->Release();
            continue;
        }

        if(socketPriv->level->value > client_priviliges)
        {
            socketPriv->Release();
            socketObj->Release();
            continue;
        }

        QJsonObject json_socket;
        json_socket.insert("type", QJsonValue(socketObj->strategy->value));
        json_socket.insert("privileges", QJsonValue(socketPriv->level->value));
        json_socket.insert("port", QJsonValue(socketObj->port->value));
        json_socket.insert("ssl", QJsonValue(socketObj->useSsl->value));

        arr.append( QJsonValue(json_socket) );

        socketPriv->Release();
        socketObj->Release();
    }

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

PrivilegeSetting *InfoStrategy::getPrivileges(QString res)
{
    PrivilegeSetting * privObj = SettingsClient::CreateReource<PrivilegeSetting>(res);

    if(!privObj->Sync(SC_OP_DEF_TIME, true))
    {
        LOG_ERROR(QString("Can't get privilege settings: '%1'").arg(res));
        privObj->Release();
        return nullptr;
    }

    return privObj;
}

AbstractSocketStrategy *createStrategy(Connection *connection)
{
    return new InfoStrategy( connection );
}
