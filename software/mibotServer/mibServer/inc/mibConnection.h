#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QString>

#include <mibSettingsDefs.h>
#include "mibAbstractSocketStrategy.h"

namespace mibot
{

class AbstractSocketStrategy;
class Connection
{    
public:
    enum ConnectionStatus {Success, Failure };

    Connection();
    ~Connection();

    QTcpSocket *        TcpSocket;
    ConnectionStatus    Status;
    QString             ErrorStrnig;

    bool                UseSsl;

    UserSetting             *UserObj;
    SocketSetting           *SocketObj;

    AbstractSocketStrategy * Strategy;

    QString             Dump(bool deep);
private:

};

}
#endif // CONNECTION_H
