#ifndef VIDEOSERVICECLIENT_H
#define VIDEOSERVICECLIENT_H

#include "Socket.h"
#include "RoverClient.h"
#include "mibJSONProtocol.h"
#include <QObject>

class PictureClient : public RoverClient
{
    Q_OBJECT
public:
    explicit PictureClient(QObject *parent = 0);
    ~PictureClient();

    bool Connect();

    void RequestPicture(int w, int h, int q);
    void onData(QByteArray data);
signals:
    void onImage(QImage *image);

protected:
    void onConnection();
    QString clientName();

private:
    QString msgTag;
    int dataToReceive;
    QByteArray dataSizeBuffer;
    QByteArray dataBuffer;
    QImage image;

    bool canReceiveData();
    bool hasMessageTag();
    void updateMessageTag(char c);
    void updateMessageSize(char c);
    QByteArray updateData(QByteArray data);
    void updateImage();
};

#endif // VIDEOSERVICECLIENT_H
