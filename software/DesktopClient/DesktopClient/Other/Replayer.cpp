#include "Clients/RoverClientsProvider.h"
#include "Other/Replayer.h"
#include "Forms/LoggerDialog.h"
#include "Settings/AppSettings.h"
#include "Forms/VideoReceiverForm.h"

Replayer::Replayer() :
    _is_replay(false),
    _is_inited(false)
{
    _nextDataChunk = 0;
}

Replayer::~Replayer()
{
    Deinitialize();
}

Replayer *Replayer::get()
{
    static Replayer obj;
    return &obj;
}

bool Replayer::InitReplayer(QString record_name)
{
    if(_is_inited)
        return true;

    QString recDir = AppSettings::GetKey(AppSettings::Recording_Store_Dir).toString();
    recDir = recDir + "/" + record_name ;
    QDir dir;

    if(!dir.exists(recDir))
    {
        LoggerDialog::get()->Error("StartReplay", QString("The record '%1'dint exists").arg(record_name));
        return false;
    }

    dir.setCurrent(recDir);
    QStringList tracksList = dir.entryList(QDir::Files);

    for(int i=0;i<tracksList.count();i++)
    {
        QString wholePath = recDir + "/" + tracksList[i];
        ReplayerClientInterface * replayInterface = GetRoverClientByName(tracksList[i]);
        if(replayInterface == nullptr)
        {
            LoggerDialog::get()->Error("Initialize replayer", "Can't create interface for track: " + tracksList[i]);
            return false;
        }
        else
        {
            TrackReplayer * track = new TrackReplayer(wholePath, replayInterface);
            if(!track->Open())
            {
                Deinitialize();
                return false;
            }
            _tracks.append(track);
        }
    }

    _is_inited = true;
    return true;
}

void Replayer::Deinitialize()
{
    for(TrackReplayer * track : _tracks)
    {
        track->Close();
        delete track;
    }

    _tracks.clear();
    _is_inited = false;
}

bool Replayer::StartReplay()
{
    qint64 nt=0;
    for(TrackReplayer * track : _tracks)
    {
        track->UpdateReplay();
        if(track->NextDataChunkTime() < nt || nt == 0)
            nt = track->NextDataChunkTime();
    }

    _replay_start_time = QDateTime::currentMSecsSinceEpoch() - nt;
    _is_replay = true;
    return true;
}

void Replayer::StopReplay()
{
    _is_replay = false;
}

bool Replayer::UpdateReplay()
{
    if(!_is_replay)
        return false;

    qint64 currentTime = QDateTime::currentMSecsSinceEpoch() - _replay_start_time;

    for(TrackReplayer * track : _tracks)
        if(track->NextDataChunkTime() <= currentTime)
            track->UpdateReplay();

    qint64 nextTime  = _tracks.first()->NextDataChunkTime();

    for(TrackReplayer * track : _tracks)
        if(track->NextDataChunkTime() < nextTime)
            nextTime = track->NextDataChunkTime();

    _nextDataChunk = nextTime;
    return true;
}

bool Replayer::IsReplayOn()
{
    return _is_replay;
}

bool Replayer::IsInitialized()
{
    return _is_inited;
}

qint64 Replayer::NextDataChunkTime()
{
    return _nextDataChunk;
}

ReplayerClientInterface *Replayer::GetRoverClientByName(QString clientName)
{
    if(clientName == "RoverSensorClient")
        return new ReplayerRoverClientInterface(RoverClientsProvider::GetRoverSensorClient());

    if(clientName == "RoverDriveClient")
        return new ReplayerRoverClientInterface(RoverClientsProvider::GetRoverDriveClient());

    if(clientName == "VideoServiceClient")
        return new ReplayerRoverClientInterface(RoverClientsProvider::GetVideoServiceClient());

    if(clientName == "Video")
        return new ReplayerVideoClientInterface();

    return nullptr;
}

TrackReplayer::TrackReplayer(QString track_path, ReplayerClientInterface *interface) :
    _track_path(track_path),
    _interface(interface),
    _eof(true)
{}

TrackReplayer::~TrackReplayer()
{
    if(_interface != nullptr)
        delete _interface;
}

bool TrackReplayer::UpdateReplay()
{
    if(_eof)
        return false;

    sendData();
    unsigned int dataSize;
    QByteArray data;

    if(!readDateTime(&_nextDataChunk))
    {
        LoggerDialog::get()->Error("TracReplay::UpdateReplay","Can't read date and time");
        return false;
    }

    if(_eof)
        return false;

    if(!readDataSize(&dataSize))
    {
        LoggerDialog::get()->Error("TracReplay::UpdateReplay","Can't read data size");
        return false;
    }

    if(_eof)
        return false;

    if(!readData(&data, dataSize))
    {
        LoggerDialog::get()->Error("TracReplay::UpdateReplay","Can't read chunk data");
        return false;
    }

    if(_eof)
        return false;

    _data_chunk = data;
    return true;
}

qint64 TrackReplayer::NextDataChunkTime()
{
    return _nextDataChunk;
}

bool TrackReplayer::Open()
{
    _track_file.setFileName(_track_path);
    if(!_track_file.open(QIODevice::ReadOnly))
    {
        LoggerDialog::get()->Error("TracReplay::Open","Can't open file for read: " + _track_path);
        return false;
    }

    _eof = false;

    return true;
}

void TrackReplayer::Close()
{
    _eof = true;
    _track_file.close();
}

void TrackReplayer::sendData()
{
    if(_data_chunk.size() == 0)
        return;

    _interface->SendData(_data_chunk);
}

bool TrackReplayer::readDateTime(qint64 *dateTime)
{
    qint64 ms = 0;
    qint64 s = _track_file.read((char*)&ms,sizeof(qint64));
    if(s == -1)
        return false;
    if(s == 0)
    {
        _eof = false;
        return true;
    }

    *dateTime = ms;
    return true;
}

bool TrackReplayer::readDataSize(unsigned int *size)
{
    qint64 s = _track_file.read((char*)size, sizeof(unsigned int));
    if(s == -1)
        return false;

    if(s == 0)
        _eof = true;

    return true;
}

bool TrackReplayer::readData(QByteArray *data, unsigned int dataSize)
{
    *data = _track_file.read(dataSize);
    if(data->size() == 0)
    {
        _eof = true;
        return true;
    }

    return data->size() == (int)dataSize;
}


ReplayerRoverClientInterface::ReplayerRoverClientInterface(RoverClient *client):
    _client(client)
{}

void ReplayerRoverClientInterface::SendData(QByteArray &data)
{
    if(_client != nullptr)
        _client->onData(data);
}

void ReplayerVideoClientInterface::SendData(QByteArray &data)
{
    VideoReceiverForm::onStaticData(data);
}
