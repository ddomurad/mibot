#include <mibLogger.h>
#include <mibSettingsClient.h>

#include "../inc/MibPictureStrategy.h"

using namespace mibot;

PictureStrategy::PictureStrategy(Connection *connection):
    AbstractSocketStrategy(connection)
{
}

PictureStrategy::~PictureStrategy()
{

}

void PictureStrategy::processNewData(QByteArray arr)
{
    _json_protocol.PushData(arr);
    fixIfJsonIsCorrupted();
    while(_json_protocol.ContainsValidJSONObject())
    {
        QJsonObject jsonObj = _json_protocol.GetPendingObject();
        processCommand(jsonObj);
        fixIfJsonIsCorrupted();
    }
}

bool PictureStrategy::init()
{
    return true;
}

void PictureStrategy::fixIfJsonIsCorrupted()
{
    if(_json_protocol.IsDataCorupted())
    {
        LOG_WARNING("StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        _json_protocol.RemoveCoruptedData();
    }
}

/*
  {
    "res_w": 320,
    "res_h": 240,
    "q": 40
  }
*/

void PictureStrategy::processCommand(QJsonObject &obj)
{
    int resW, resH, q;
    if( !obj["res_w"].isDouble()
     || !obj["res_h"].isDouble()
     || !obj["q"].isDouble())
    {
        LOG_ERROR("Invalid message format.");
        sendError("Invalid message format.");
        return;
    }

    resW = (int)obj["res_w"].toDouble();
    resH = (int)obj["res_h"].toDouble();
    q = (int)obj["q"].toDouble();

    QString tmpPicturePath = "/home/mibot/tmp/test.jpg";

    LOG_INFO(QString("Taking picture: %1x%2 q:%3")
             .arg(resW)
             .arg(resH)
             .arg(q));

    if(!takePicture(resW, resH, q, tmpPicturePath))
        return;

    sendPicture(tmpPicturePath);
}

void PictureStrategy::sendError(QString str)
{
    if(str.length() > 255)
        str = str.left(255);

    char strSize[1];
    strSize[0] = (char)str.length();

    _connection->TcpSocket->write("ERR");
    _connection->TcpSocket->write(strSize);
    _connection->TcpSocket->write(str.toUtf8());
}

void PictureStrategy::sendPicture(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        LOG_ERROR("Can't open file to read: " + path);
        sendError("Can't open file to read: " + path);
        return;
    }

    QByteArray imageData = file.readAll();
    int fileSize = imageData.size();
    file.close();

    _connection->TcpSocket->write("IMG");
    _connection->TcpSocket->write((const char*)&fileSize, 4);
    _connection->TcpSocket->write(imageData);
}

bool PictureStrategy::takePicture(int w, int h, int q, QString path)
{
    QString systemCommand = "/opt/vc/bin/raspistill -w %1 -h %2 -o %3 -q %4";
    systemCommand = systemCommand.arg(w).arg(h).arg(path).arg(q);
    system(systemCommand.toStdString().c_str());
    return true;
}
