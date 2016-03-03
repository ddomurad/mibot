#ifndef ROVERSENSORCLIENT_H
#define ROVERSENSORCLIENT_H

#include "Socket.h"
#include <QObject>
#include "RoverClient.h"
#include "mibJSONProtocol.h"

class SystemSensors
{
public:
    float cpu_temp;
    float cpu_usage_total;
    float cpu_usage_server;
    float mem_available;
    float mem_usage_total;
    float mem_usage_server;
};

class ArduinoSensors
{
   public:
    int acc[3];
    int mag[3];

    float analogValue[10];
    bool hasAnalogValue[10];

    int us;
};

class RoverSensors
{
public:
    SystemSensors systemSensors;
    ArduinoSensors arduinoSensors;
};

class RoverSensorClient : public RoverClient
{
    Q_OBJECT
public:
    explicit RoverSensorClient(QObject *parent = 0);
    ~RoverSensorClient();

    bool Connect();

signals:
    void newData(RoverSensors readings);
public slots:

private:

protected:
    void onData(QByteArray data);

private:
    void processObj(QJsonObject & obj);
    mibot::JSONProtocol protocol;

    void fixIfJsonIsCorrupted();
};

#endif // ROVERSENSORCLIENT_H
