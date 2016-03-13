#ifndef MIBGPS_H
#define MIBGPS_H

#include "mibGpsGlobal.h"
#include <QtCore>

namespace mibot
{

class MIBGPSSHARED_EXPORT GPSPosition
{
public:
    GPSPosition();

    double latitude;
    double lognitude;
    QChar northSouth;
    QChar eastWast;
};

class MIBGPSSHARED_EXPORT GPSMovement
{
public:
    GPSMovement();

    double speedKnot;
    double speedKmh;
    double speedMs;
    double course;
};

class MIBGPSSHARED_EXPORT GPSData
{
public:
    GPSPosition position;
    GPSMovement movement;
};

class MIBGPSSHARED_EXPORT GPSPositionDataParser
{
public:
    GPSPositionDataParser();
    bool CanParse(QString line);
    bool Parse(QString line);

    QDateTime DateTimeStamp();
    bool IsValid();
    GPSData GpsData();

private:
    QDateTime _dateTimeStamp;
    bool _isValid;
    GPSData _gpsData;
};

}
#endif // MIBGPS_H
