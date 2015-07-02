#include <QJsonObject>
#include "../inc/mibSettingsServer.h"
#include "../inc/mibLocalDBHandler.h"
#include <mibLogger.h>

using namespace mibot;

ClientJSONProtocol::ClientJSONProtocol():
    protocl(new JSONProtocol())
{

}

ClientJSONProtocol::~ClientJSONProtocol()
{
    delete protocl;
}

ClientJSONProtocol::ClientJSONProtocol(const ClientJSONProtocol &obj)
{
    client = obj.client;
    protocl = obj.protocl;
}


SettingsServer::SettingsServer(QObject * parent) :
    QObject(parent)
{
    _listener = nullptr;
    _dbHandler = nullptr;
}

SettingsServer::~SettingsServer()
{
    if(_dbHandler != nullptr)
        delete _dbHandler;
}

bool SettingsServer::startServer(QJsonObject &settings)
{
    QJsonObject dbSettigns = settings["DBHandler"].toObject();

    LocalDBHandler * localdbHandler = new LocalDBHandler();
    if(!localdbHandler->InitReader(dbSettigns))
    {
        delete localdbHandler;
        return false;
    }
    _dbHandler = localdbHandler;

    QJsonObject listenerSettigns = settings["Listener"].toObject();
    _listener = new ClientListener(this);
    connect(_listener, SIGNAL(newClient(QTcpSocket*)), this, SLOT(newClient(QTcpSocket*)));
    if(!_listener->startListen(listenerSettigns))
        return false;


    return true;
}

void SettingsServer::stop()
{
    _listener->stopListen();
    for(int i=0;i<_clients.count(); i++)
    {
        _clients[i]->client->close();
        _clients[i]->client->deleteLater();
        delete _clients[i];
    }

    _clients.clear();
}

void SettingsServer::newClient(QTcpSocket *socket)
{
    connect(socket, SIGNAL(disconnected()), this , SLOT(clientDisconnected()));
    connect(socket, SIGNAL(readyRead()), this , SLOT(onClientData()));

    ClientJSONProtocol * newClient = new ClientJSONProtocol();
    newClient->client = socket;
    _clients.append( newClient );
}

void SettingsServer::clientDisconnected()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == nullptr) return;
    int index = getClientIndex(socket);
    if(index == -1) return;
    LOG_INFO("Client disconnected: " + socket->peerAddress().toString());

    _clients.removeAt(index);
}

void SettingsServer::onClientData()
{
    QTcpSocket * socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == nullptr) return;

    JSONProtocol * protocol = getClientProtocol(socket);
    QByteArray data =  socket->readAll();
    protocol->PushData( data );
    processCommands( protocol, socket );
}

int SettingsServer::getClientIndex(QTcpSocket *socket)
{
    for(int i=0; i< _clients.count(); i++)
        if(_clients[i]->client == socket)
            return i;
    return -1;
}

JSONProtocol *SettingsServer::getClientProtocol(QTcpSocket *socket)
{
    int index = getClientIndex(socket);
    if(index == -1) return nullptr;
    return _clients[index]->protocl;
}

void SettingsServer::repairJsonData(JSONProtocol *protocol)
{
    if(protocol->IsDataCorupted())
    {
        LOG_WARNING("JSON protcol data corupted. Trying to repair.");
        protocol->RemoveCoruptedData();
        if(protocol->IsDataCorupted())
        {
            LOG_ERROR("JSON protcol data corupted. Can't repair, all data will be deleted!");
            protocol->RemoveAllData();
        }
    }
}

void SettingsServer::processCommands(JSONProtocol *protocol, QTcpSocket *client)
{
    repairJsonData( protocol );
    while(protocol->ContainsValidJSONObject())
    {
        QJsonObject command = protocol->GetPendingObject();
        processCommand( command , client);

        repairJsonData( protocol );
    }
}

void SettingsServer::processCommand(QJsonObject &obj, QTcpSocket *client)
{
    QString resoruce = obj["Resource"].toString();
    if(resoruce.isEmpty())
    {
        sendNack("Invalid command. Resource is empty!", "Unknown", "", client);
        return;
    }

    QString command = obj["Command"].toString();

    QJsonObject data = obj["Data"].toObject();

    if(command.isEmpty())
    {
        LOG_ERROR("Invalid command. Command is empty!");
        sendNack("Invalid command. Command is empty!", "Unknown", resoruce, client);
        return;
    }
    else if(command == "Get")
    {
       QJsonObject resp;
       QString error;
       if(!_dbHandler->GetResource(resoruce, &resp, client,&error))
       {
           sendNack("Get error: " + error, "Get-Resp", resoruce, client);
           return;
       }
       sendRespWithDecoration(resoruce, "Get-Resp", resp, client);
       return;
    }
    else if(command == "Set")
    {
        if(data.isEmpty())
        {
            sendNack("Invalid command. Data is empty!", "Set", resoruce, client);
            return;
        }

        QString error;
        if(!_dbHandler->SetResource(resoruce, &data, client, & error))
        {
            sendNack("Set error: " + error, "Set-Resp", resoruce, client);
            return;
        }
        sendAck("Set-Resp", resoruce, client);
        return;
    }
    else if(command == "Remove")
    {
        QString error;
        if(!_dbHandler->RemoveResource(resoruce,client, &error))
        {
            sendNack("Remove error: " + error, "Remove-Resp", resoruce, client);
            return;
        }
        sendAck("Remove-Resp", resoruce, client);
        return;
    }else if(command == "List")
    {
        QString error;
        QJsonObject results;
        if(!_dbHandler->ListResources(resoruce, &results, client, &error))
        {
            sendNack("List error: " + error, "List-Resp", resoruce, client);
            return;
        }
        sendRespWithDecoration(resoruce, "List-Resp", results, client);
        return;
    }else if(command == "Subscribe")
    {
        _dbHandler->SubscribeForChangeNotification(resoruce, client);
        sendAck("Subscribe-Resp", resoruce, client);
    }
    else if(command == "Unsubscribe")
    {
        _dbHandler->UnsubscribeFromChangeNotification(resoruce, client);
        sendAck("Unsubscribe-Resp", resoruce, client);
    }
    else
    {
        LOG_ERROR(QString("Invalid command. Unsuported command: '%1'").arg(command));
        return;
    }
}

void SettingsServer::sendNack(QString message, QString commnand, QString resource, QTcpSocket *client)
{
    QJsonObject resp;
    resp.insert("Resp", QJsonValue("NACK"));
    resp.insert("Commnand", QJsonValue(commnand));
    resp.insert("Message", QJsonValue(message));
    resp.insert("Resource", QJsonValue(resource));

    LOG_WARNING( QString("Sending Nack: (Command: %1, Message: %2)").arg(commnand, message) );

    QJsonDocument doc(resp);
    client->write( doc.toJson(QJsonDocument::Compact) );
}

void SettingsServer::sendAck(QString commnand, QString resource, QTcpSocket *client)
{
    QJsonObject resp;
    resp.insert("Resp", QJsonValue("ACK"));
    resp.insert("Command", QJsonValue(commnand));
    resp.insert("Resource", QJsonValue(resource));

    QJsonDocument doc(resp);
    client->write( doc.toJson(QJsonDocument::Compact) );
}

void SettingsServer::sendRespWithDecoration(QString resource, QString command, QJsonObject &obj, QTcpSocket * client)
{
    QJsonObject finalObject;
    finalObject.insert("Resp", QJsonValue("ACK"));
    finalObject.insert("Resource", QJsonValue(resource));
    finalObject.insert("Command", QJsonValue(command));
    finalObject.insert("Data", QJsonValue(obj));

    QJsonDocument doc( finalObject );
    client->write( doc.toJson(QJsonDocument::Compact) );
}

