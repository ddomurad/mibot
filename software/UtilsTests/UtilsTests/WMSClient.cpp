#include <QDebug>
#include "WMSClient.h"

WMSClient::WMSClient(QCoreApplication * app) : QObject(app)
{
    manager = new QNetworkAccessManager(this);
//    connect(manager, SIGNAL(finished(QNetworkReply*)),
//            this, SLOT(replyFinished(QNetworkReply*)));
}

WMSClient::~WMSClient()
{

}
/*
 * http://sampleserver1.arcgisonline.com/arcgis/services/Specialty/ESRI_StatesCitiesRivers_USA/MapServer/WMSServer?REQUEST=GetFeatureInfo&SERVICE=WMS&VERSION=1.1.1&LAYERS=0&STYLES=default&FORMAT=image/png&BGCOLOR=0xFFFFFF&TRANSPARENT=TRUE&SRS=EPSG:4326&BBOX=-125.192865,11.2289864971264,-66.105824,62.5056715028736&WIDTH=1044&HEIGHT=906&QUERY_LAYERS=0&X=103&Y=462
*/

void WMSClient::Test()
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://a.tile.opencyclemap.org/cycle/10/566/348.png"));
//    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    QNetworkReply * reply = manager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished2()));
}

void WMSClient::slotError(QNetworkReply::NetworkError e)
{
    qDebug() << "slotError: " << e;
}

void WMSClient::slotSslErrors(QList<QSslError> e)
{
    qDebug() << "----------- ssl errors -------------";
    for( auto error : e )
    {
        qDebug() << "sslError: " << error;
    }
}

void WMSClient::replyFinished(QNetworkReply * reply)
{
    QByteArray arr = reply->readAll();
    qDebug() << arr.toHex();
}

void WMSClient::replyFinished2()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray arr = reply->readAll();
    qDebug() << arr.toHex();
    reply->deleteLater();
}

