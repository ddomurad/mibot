#ifndef MIBOTGPSSENSOR_H
#define MIBOTGPSSENSOR_H

#include "mibSensorsGlobal.h"
#include <QObject>
#include <mibGPSDataParser.h>
#include "mibSensorsSettings.h"
#include <QSerialPort>

namespace mibot
{

class MIBSENSORSSHARED_EXPORT GPSSensor : public QObject
{
    Q_OBJECT
public:
    static GPSSensor * get();
    ~GPSSensor();

    bool Start();
    void Stop();

    GPSData Readings();
private slots:
    void onSerialData();

private:
    GPSSensor(QObject * parent);
    GPSPositionDataParser _gps_data_parser;
    SensorsSettings *_settings;

    bool _is_started;

    QSerialPort *_serialPort;

    void parseData(QString line);
    GPSData _gps_data;

    QMutex _mutex;
};
}
#endif // MIBOTGPSSENSOR_H
