#include "inc/mibStreamer.h"
#include <QFile>

using namespace mibot;
Streamer::Streamer(QObject *parent) : QObject(parent)
{}

Streamer::~Streamer()
{}

void Streamer::SetParams(QString device, int w, int h)
{
  QString cmd = "v4l2-ctl --set-fmt-video=width=%1,height=%2,pixelformat=4 -d %3";
  cmd = cmd.arg(w);
  cmd = cmd.arg(h);
  cmd = cmd.arg(device);

 _device = device;
}

void Streamer::StreamStart()
{
    QFile file(_device);
    if(file.open(QIODevice::ReadOnly))
    {
        emit OpenDeviceError();
        return;
    }

    _shall_stream = true;

    emit StreamingStarted();
    while(_shall_stream)
    {
        StreamData data;
        data.data_len = file.read(data.data,STREAM_DATA_MAX_CHUNK_SIZE);
        if(data.data_len == -1)
        {
            emit DeviceReadError();
            _shall_stream = false;
            break;
        }
        emit DataReady(data);
    }

    file.close();
    emit StreamingStoped();
}

void Streamer::StreamStop()
{
    _shall_stream = false;
}

