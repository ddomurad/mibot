#ifndef MIBGPS_H
#define MIBGPS_H

#include "mibGpsGlobal.h"
#include <QtCore>

namespace mibot
{

class MIBGPSSHARED_EXPORT IGPSDataParser
{
    virtual bool CanParse(QString line);
    virtual bool Parse(QString line);
};

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

class MIBGPSSHARED_EXPORT GPSPositionDataParser
{
public:
    GPSPositionDataParser();
    bool CanParse(QString line);
    bool Parse(QString line);

    QDateTime DateTimeStamp();
    bool IsValid();
    GPSPosition Position();
    GPSMovement Movement();

private:
    QDateTime _dateTimeStamp;
    bool _isValid;
    GPSPosition _position;
    GPSMovement _movement;
};

}
#endif // MIBGPS_H
