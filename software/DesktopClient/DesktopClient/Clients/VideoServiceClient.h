#ifndef VIDEOSERVICECLIENT_H
#define VIDEOSERVICECLIENT_H

#include "Socket.h"
#include "RoverClient.h"
#include "mibJSONProtocol.h"
#include <QObject>

class VideoServiceClient : public RoverClient
{
    Q_OBJECT
public:
    explicit VideoServiceClient(QObject *parent = 0);
    ~VideoServiceClient();

    bool Connect();

    void StartStreamer(int w, int h, QString device, QString serverAddr, int serverPort, QString format, int buffer);
    void StopStreamer();
    void onData(QByteArray data);

protected:
    void onConnection();
    QString clientName();

private:
    void processObj(QJsonObject & obj);
    mibot::JSONProtocol protocol;

    void fixIfJsonIsCorrupted();

};

#endif // VIDEOSERVICECLIENT_H
