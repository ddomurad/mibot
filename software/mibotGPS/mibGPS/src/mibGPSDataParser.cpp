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
    latitude(0.0f), longitude(0.0f)
{}

GPSMovement::GPSMovement():
    speedKmh(0.0f), course(0.0f)
{}

GPSData::GPSData() :
    isValid(false)
{}


GPSPositionDataParser::GPSPositionDataParser()
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

//    QString date_time_string = splited[9] + splited[1];
//    _dateTimeStamp = QDateTime::fromString(date_time_string, "ddMMyyHHmmss.zzz").addYears(100);

    _gpsData.isValid = splited[2] == "A";
    bool ok = true;

    _gpsData.position.latitude = _gpsCoordToDouble(splited[3]);
    _gpsData.position.latitude *= splited[4] == "N" ? 1.0 : -1.0;

    _gpsData.position.longitude = _gpsCoordToDouble(splited[5]);
    _gpsData.position.longitude *= splited[6] == "E" ? 1.0 : -1.0;

    _gpsData.movement.speedKmh = splited[7].toDouble(&ok) * 1.852f;

    if(!ok) return false;
    _gpsData.movement.course = splited[8].toDouble(&ok);

    return true;
}

//QDateTime GPSPositionDataParser::DateTimeStamp()
//{
//    return _dateTimeStamp;
//}

GPSData GPSPositionDataParser::GpsData()
{
    return _gpsData;
}

double GPSPositionDataParser::_gpsCoordToDouble(QString str)
{
    int dotIndex = str.indexOf('.');
    if(dotIndex == -1)
        return 0;

    QString deg = str.left( dotIndex - 2);
    QString m = str.right( str.length() - dotIndex + 2);

    return deg.toDouble() + m.toDouble() / 60.0;
}
