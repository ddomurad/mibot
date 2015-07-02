#ifndef MIBMIBSETTINGSSERVER_H
#define MIBMIBSETTINGSSERVER_H

#include <QObject>
#include "mibSettingsServerGlobal.h"
#include "mibClientListener.h"
#include "mibAbstractDataBaseHandler.h"
#include "mibJSONProtocol.h"

namespace mibot
{

class MIBSETTINGSSERVERSHARED_EXPORT ClientJSONProtocol
{
public:
    ClientJSONProtocol();
    ~ClientJSONProtocol();

    ClientJSONProtocol(const ClientJSONProtocol &obj);

    QTcpSocket * client;
    JSONProtocol *protocl;
};

class MIBSETTINGSSERVERSHARED_EXPORT SettingsServer : public QObject
{
    Q_OBJECT
public:
    explicit SettingsServer(QObject * parent = nullptr);
    ~SettingsServer();

    bool startServer(QJsonObject & settings);
    void stop();

private slots:
    void newClient(QTcpSocket * socket);
    void clientDisconnected();

    void onClientData();

private:
    ClientListener * _listener;
    QList<ClientJSONProtocol*> _clients;
    AbstractDataBaseHandler * _dbHandler;

    int getClientIndex(QTcpSocket * socket);
    JSONProtocol * getClientProtocol(QTcpSocket * socket);

    void repairJsonData(JSONProtocol * protocol);
    void processCommands(JSONProtocol * protocol, QTcpSocket * client);
    void processCommand(QJsonObject & obj, QTcpSocket * client);

    void sendNack(QString message, QString commnand, QString resource, QTcpSocket * client);
    void sendAck(QString commnand, QString resource, QTcpSocket * client);

    void sendRespWithDecoration(QString resource, QString command, QJsonObject & obj, QTcpSocket *client);
};

}

/*
{
 "Command":"Set",
 "Resource":"imite_1.json",
 "Data":
    {
        "Imit":"Daniel",
        "Nazwisko":"Daniel"
    }
}

{
    "Command":"Get",
    "Resource":"imite_1.json"
}

{
    "Command":"List",
    "Resource":"."
}

{
    "Command":"Subscribe",
    "Resource":"imite_1.json"
}

{
    "Command":"Remove",
    "Resource":"imite_1.json"
}

*/
#endif // MIBMIBSETTINGSSERVER_H
