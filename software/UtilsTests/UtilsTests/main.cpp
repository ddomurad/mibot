#include <QCoreApplication>
#include <qobject.h>
#include <QFile>
#include <QDebug>
#include <cstdio>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QVariant>

#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibGPSDataParser.h>

#include <QHttpPart>

#include "Reader.h"
#include <WMSClient.h>

double long2tilex(double lon, int z)
{
    return (lon + 180.0) / 360.0 * pow(2.0, z);
}

double  lat2tiley(double lat, int z)
{
    return (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z);
}

double tilex2long(int x, int z)
{
    return x / pow(2.0, z) * 360.0 - 180;
}

double tiley2lat(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc,argv);
    //SerialReader* reader = new SerialReader(&a);

    WMSClient * client = new WMSClient(&a);
    client->Test();
    a.exec();
}
