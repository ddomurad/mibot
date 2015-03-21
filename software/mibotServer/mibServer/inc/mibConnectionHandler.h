#ifndef CONNECTIONHANDLER_H
#define CONNECTIONHANDLER_H

#include <QObject>
#include "mibConnection.h"
#include <mibAccessResources.h>

namespace mibot
{

class ConnectionHandler : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionHandler(QObject *parent = 0);
    ~ConnectionHandler();

signals:

public slots:
    void NewConnection(Connection *);

private slots:
    void onDisconnected();

private:
    Connection *    connectionFromSocket(QTcpSocket *socket);
    void            addConnection(Connection *connection);
    void            removeConnection(Connection *connection);
    void            pushAndReleaseAudit(Connection *connection, QString entry_type, QString entry_desc);

    QList<Connection *> _connections;
};

}
#endif // CONNECTIONHANDLER_H
