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
};

class MIBGPSSHARED_EXPORT GPSMovement
{
public:
    GPSMovement();

    double speedKmh;
    double course;
};

class MIBGPSSHARED_EXPORT GPSData
{
public:
    GPSData();
    bool isValid;
    GPSPosition position;
    GPSMovement movement;
};

class MIBGPSSHARED_EXPORT GPSPositionDataParser
{
public:
    GPSPositionDataParser();
    bool CanParse(QString line);
    bool Parse(QString line);

    GPSData GpsData();

private:
    GPSData _gpsData;

    double _gpsCoordToDouble(QString str);
};

}
#endif // MIBGPS_H
