#ifndef ROVERDRIVECLIENT_H
#define ROVERDRIVECLIENT_H

#include <QTimer>
#include "./Clients/RoverClient.h"
#include "Other/JsInput.h"

class RoverDriveClient : public RoverClient
{
    Q_OBJECT
public:
    RoverDriveClient(QObject * parent = nullptr);
    ~RoverDriveClient();

    void SendDriveCommand(int da, int ta, bool brake, bool turbo);

//signals:
//    void SendDriveData(uchar *data);
//private slots:
//    void onSendDriveData(uchar *data);

protected:
    void onData(QByteArray data);
    void onConnection();

    // RoverClient interface
public:
    bool Connect();
};

#endif // ROVERDRIVECLIENT_H
