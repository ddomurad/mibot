#ifndef STREAMER_H
#define STREAMER_H

#include <QObject>

namespace mibot
{

#define STREAM_DATA_MAX_CHUNK_SIZE 512

class StreamData
{
public:
    char data[STREAM_DATA_MAX_CHUNK_SIZE];
    unsigned int data_len;
};

class Streamer : public QObject
{
    Q_OBJECT
public:
    explicit Streamer(QObject *parent = 0);
    ~Streamer();

    void SetParams(QString device, int w, int h);
signals:
    void DataReady(StreamData data);
    void OpenDeviceError();
    void DeviceReadError();
    void StreamingStarted();
    void StreamingStoped();

public slots:
    void StreamStart();
    void StreamStop();

private:
    QString _device;
    bool _shall_stream;
};

}
#endif // STREAMER_H
