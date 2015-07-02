#include <mibLogger.h>
#include <QHostAddress>
#include "inc/mibAbstractDataBaseHandler.h"

using namespace mibot;

void AbstractDataBaseHandler::SubscribeForChangeNotification(QString resource, QTcpSocket *client)
{
    if(!_notificationMap.contains( resource ))
        _notificationMap.insert(resource, QList<QTcpSocket*>());

    if(!_notificationMap[resource].contains(client))
    {
        LOG_INFO(QString("Add resource '%1' to subscription for client: %2")
                 .arg(resource).arg(client->peerAddress().toString()));
        _notificationMap[resource].append(client);
    }
}

void AbstractDataBaseHandler::UnsubscribeFromChangeNotification(QString resource, QTcpSocket *client)
{
    if(!_notificationMap.contains(resource)) return;
    if(!_notificationMap[resource].contains(client)) return;

    _notificationMap[resource].removeAll(client);

    LOG_INFO(QString("Remove resource '%1' from subscription for client: %2")
             .arg(resource).arg(client->peerAddress().toString()));
}

void AbstractDataBaseHandler::UnsubscribeAll(QString resource)
{
    if(_notificationMap.contains(resource))
        _notificationMap.remove(resource);
}

void AbstractDataBaseHandler::notifySubscribers(QString resource, QJsonObject &obj)
{
    QList<QTcpSocket*> socketsToRemove;
    for(QTcpSocket * client : _notificationMap[resource])
    {
        QJsonObject finalObj;
        finalObj.insert("Resp", "ACK");
        finalObj.insert("Resource", QJsonValue(resource));
        finalObj.insert("Command", QJsonValue("Change"));
        finalObj.insert("Data", QJsonValue(obj));

        QJsonDocument doc(finalObj);
        if(client->write(doc.toJson(QJsonDocument::Compact)) == -1)
            socketsToRemove.append(client);
    }

    for(QTcpSocket * socket : socketsToRemove)
        _notificationMap[resource].removeAll(socket);

    if(_notificationMap[resource].isEmpty())
        _notificationMap.remove(resource);
}
