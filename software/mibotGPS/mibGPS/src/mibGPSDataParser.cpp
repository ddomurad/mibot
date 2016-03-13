#include "../inc/mibGPSDataParser.h"

using namespace mibot;

/*
$GPGSA,A,1,,,,,,,,,,,,,0.0,0.0,0.0*30
$GPGSV,1,1,02,06,00,000,52,12,00,000,45*78
$GPRMC,115951.000,V,0000.0000,N,00000.0000,E,000.0,000.0,280606,,,N*74
$GPVTG,000.0,T,,M,000.0,N,000.0,K,N*02
$GPGGA,115952.000,0000.0000,N,00000.0000,E,0,00,0.0,0.0,M,0.0,M,,0000*66
*/

GPSPosition::GPSPosition():
    latitude(0.0f), lognitude(0.0f),
    northSouth('?'), eastWast('?')
{}

GPSMovement::GPSMovement():
    speedKnot(0.0f), speedKmh(0.0f),
    speedMs(0.0f), course(0.0f)
{}


GPSPositionDataParser::GPSPositionDataParser() :
    _isValid(false)
{}

bool GPSPositionDataParser::CanParse(QString line)
{
    return line.startsWith("$GPRMC");
}

bool GPSPositionDataParser::Parse(QString line)
{
    if(!line.startsWith("$GPRMC")) return false;

    line = line.trimmed();

    // GPRMC,115951.000,V,0000.0000,N,00000.0000,E,000.0,000.0,280606,,,N*74
    QStringList splited = line.split(',');
    if(splited.count() != 13) return false;

    QString date_time_string = splited[9] + splited[1];
    _dateTimeStamp = QDateTime::fromString(date_time_string, "ddMMyyHHmmss.zzz").addYears(100);;

    _isValid = splited[2] == "A";
    bool ok = true;

    _gpsData.position.latitude = splited[3].toDouble(&ok);
    if(!ok) return false;

    if(splited[4].length() != 1)
        _gpsData.position.northSouth = '?';
    else
        _gpsData.position.northSouth = splited[4][0];

    _gpsData.position.lognitude = splited[5].toDouble(&ok);
    if(!ok) return false;

    if(splited[6].length() != 1)
        _gpsData.position.eastWast = '?';
    else
        _gpsData.position.eastWast = splited[6][0];

    _gpsData.movement.speedKnot = splited[7].toDouble(&ok);
    if(!ok) return false;

    _gpsData.movement.speedKmh = _gpsData.movement.speedKnot * 1.852f;
    _gpsData.movement.speedMs = _gpsData.movement.speedKnot * 0.514444f;

    if(!ok) return false;
    _gpsData.movement.course = splited[8].toDouble(&ok);

    return true;
}

QDateTime GPSPositionDataParser::DateTimeStamp()
{
    return _dateTimeStamp;
}

bool GPSPositionDataParser::IsValid()
{
    return _isValid;
}

GPSData GPSPositionDataParser::GpsData()
{
    return _gpsData;
}
