#include "./Settings/AppSettings.h"
#include "PictureClient.h"
#include "./Forms/LoggerDialog.h"

PictureClient::PictureClient(QObject *parent) : RoverClient(parent)
{    
    msgTag = "";
    dataToReceive = 0;
}

PictureClient::~PictureClient()
{

}

bool PictureClient::Connect()
{
    return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Picture_Service).toString());
}

void PictureClient::RequestPicture(int w, int h, int q)
{
    QString data = QString("{ \
    \"res_w\":%1,\
    \"res_h\":%2,\
    \"q\":%3\
    }");

    data = data.arg(w);
    data = data.arg(h);
    data = data.arg(q);

    SendData(data);
}

void PictureClient::onData(QByteArray data)
{
    while(data.length() > 0)
    {
        if(!canReceiveData())
        {
            if(!hasMessageTag())
            {
                char c = data.at(0);
                data = data.right(data.length() - 1);
                updateMessageTag(c);
            }else
            {
                char c = data.at(0);
                data = data.right(data.length() - 1);
                updateMessageSize(c);
            }
        }else
        {
            data = updateData(data);
        }
    }
}

void PictureClient::onConnection()
{}

QString PictureClient::clientName()
{
    return "PictureClient";
}

bool PictureClient::canReceiveData()
{
    return hasMessageTag() && dataToReceive > 0;
}

bool PictureClient::hasMessageTag()
{
    return msgTag == "ERR" || msgTag == "IMG";
}

void PictureClient::updateMessageTag(char c)
{
    dataToReceive = 0;
    dataSizeBuffer.clear();
    dataBuffer.clear();

    if(msgTag == "" && c=='E')
    {
        msgTag += "E"; return;
    }

    if(msgTag == "" && c=='I')
    {
        msgTag += "I"; return;
    }

    if(msgTag == "E" && c == 'R')
    {
        msgTag += "R"; return;
    }

    if(msgTag == "ER" && c == 'R')
    {
        msgTag += "R"; return;
    }

    if(msgTag == "I" && c == 'M')
    {
        msgTag += "M"; return;
    }

    if(msgTag == "IM" && c == 'G')
    {
        msgTag += "G"; return;
    }

    msgTag = "";
}

void PictureClient::updateMessageSize(char c)
{
    if(msgTag == "ERR")
    {
        dataToReceive = c;
    }

    if(msgTag == "IMG")
    {
        dataSizeBuffer.append(c);
        if(dataSizeBuffer.length() == 4)
        {
            char *s = (char*)&dataToReceive;
            s[0] = dataSizeBuffer.at(0);
            s[1] = dataSizeBuffer.at(1);
            s[2] = dataSizeBuffer.at(2);
            s[3] = dataSizeBuffer.at(3);
        }
    }
}

QByteArray PictureClient::updateData(QByteArray data)
{
    if(data.length() < dataToReceive)
    {
        dataToReceive-=data.length();
        dataBuffer.append(data);
        return QByteArray();
    }else
    {
      QByteArray retData = data.right(data.length() - dataToReceive);
      dataBuffer.append(data.left(dataToReceive));
      dataToReceive = 0;
      msgTag = "";

      updateImage();

      return retData;
    }
}

void PictureClient::updateImage()
{
    image = QImage::fromData(dataBuffer);
    dataBuffer.clear();
    emit onImage(&image);
}

