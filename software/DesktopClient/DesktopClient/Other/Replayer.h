#ifndef REPLAYER_H
#define REPLAYER_H

#include <QtCore>
#include "Clients/RoverClient.h"

class ReplayerClientInterface
{
  public:
    virtual ~ReplayerClientInterface() = default;
    virtual void SendData(QByteArray &data) = 0;
};

class ReplayerRoverClientInterface : public ReplayerClientInterface
{
public:
    ReplayerRoverClientInterface(RoverClient * client);
    ~ReplayerRoverClientInterface() = default;
    void SendData(QByteArray &data);
private:
    RoverClient * _client;
};

class ReplayerVideoClientInterface : public ReplayerClientInterface
{
public:
    void SendData(QByteArray &data);
};

class TrackReplayer
{
public:
    TrackReplayer(QString track_path, ReplayerClientInterface * interface);
    virtual ~TrackReplayer();

    bool UpdateReplay();
    qint64 NextDataChunkTime();

    bool Open();
    void Close();

private:
    void sendData();
    QString _track_path;
    qint64 _nextDataChunk;
    QFile _track_file;

    QByteArray _data_chunk;
    bool readDateTime(qint64 *dateTime);
    bool readDataSize(unsigned int *size);
    bool readData(QByteArray *data, unsigned int dataSize);

    ReplayerClientInterface * _interface;
};

class Replayer
{
public:
    ~Replayer();
    static Replayer * get();

    bool InitReplayer(QString record_name);
    void Deinitialize();
    bool StartReplay();
    void StopReplay();

    bool UpdateReplay();
    bool IsReplayOn();
    bool IsInitialized();

    qint64 NextDataChunkTime();

private:
    Replayer();
    bool _is_replay;
    bool _is_inited;

    qint64 _nextDataChunk;
    QList<TrackReplayer*> _tracks;

    ReplayerClientInterface * GetRoverClientByName(QString clientName);
};

#endif // REPLAYER_H
