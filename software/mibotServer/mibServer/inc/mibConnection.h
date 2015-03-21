#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpSocket>
#include <QString>

#include <mibGlobalAccess.h>
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

    QUuid               Id;
    QUuid               User;
    QUuid               Socket;
    QTcpSocket *        TcpSocket;
    ConnectionStatus    Status;
    QString             ErrorStrnig;

    bool                UseSsl;
//    QString             LocalCertificatePath;
//    QString             LocalKeyPath;
//    QString             CaCertificatesPath;

    UserRes             *UserObj();
    SocketRes           *SocketObj();

    AbstractSocketStrategy * Strategy;

    QString             Dump(bool deep);
private:
    UserRes             *_user;
    SocketRes           *_socket;
};

}
#endif // CONNECTION_H
