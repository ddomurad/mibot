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
    void onData(QByteArray);

protected:
    void onConnection();
    QString clientName();
    // RoverClient interface
public:
    bool Connect();


};

#endif // ROVERDRIVECLIENT_H
