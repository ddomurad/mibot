#include "./Settings/AppSettings.h"
#include "RoverDriveClient.h"

RoverDriveClient::RoverDriveClient(QObject *parent)
    :RoverClient(parent)
{}

RoverDriveClient::~RoverDriveClient()
{

}

void RoverDriveClient::SendDriveCommand(int da, int ta, bool brake, bool turbo)
{
    qint8 left  =   100 * ( float(da)/32767.0f );
    qint8 right =   100 * ( float(ta)/32767.0f );
    uchar data[] =
    {
        0x30, // write cmd
        0x00, // addr
        // driver model ( 0000 100S )
        (uchar)( brake ? 0x01 : 0x00 ),
        (uchar)( turbo ? 0x01 : 0x00 ),
        (uchar)left,
        (uchar)right
    };

    SendData(data, 6);
}

void RoverDriveClient::onData(QByteArray)
{}

void RoverDriveClient::onConnection()
{}

bool RoverDriveClient::Connect()
{
    return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Driver_Service).toString());
}
