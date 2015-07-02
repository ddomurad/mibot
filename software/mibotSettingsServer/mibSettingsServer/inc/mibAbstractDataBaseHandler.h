#ifndef ABSTRACTDATABASEREADER_H
#define ABSTRACTDATABASEREADER_H

#include <QtCore>
#include <QTcpSocket>

namespace mibot
{
class AbstractDataBaseHandler
{
protected:
    AbstractDataBaseHandler() {}

public:
    virtual ~AbstractDataBaseHandler() {}

    virtual bool SetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString * outErrror) = 0;
    virtual bool GetResource(QString resource, QJsonObject *obj, QTcpSocket * caller, QString * outErrror) = 0;
    virtual bool RemoveResource(QString resource, QTcpSocket * caller, QString * outErrror) = 0;
    virtual bool ListResources(QString resourceDir, QJsonObject *obj, QTcpSocket * caller, QString * outErrror) = 0;

    void SubscribeForChangeNotification(QString resource, QTcpSocket * client);
    void UnsubscribeFromChangeNotification(QString resource, QTcpSocket * client);
    void UnsubscribeAll(QString resource);

protected:
    void notifySubscribers(QString resource, QJsonObject & obj);

private:
    QMap<QString, QList<QTcpSocket*>> _notificationMap;
};

}

#endif // ABSTRACTDATABASEREADER_H
