#include "Other/Recorder.h"

Recorder::Recorder():
    _is_recording(false)
{}

Recorder::~Recorder()
{}

Recorder *Recorder::get()
{
    static Recorder rec;
    return &rec;
}

bool Recorder::StartRecording(QString record_name)
{
    QString recDir = AppSettings::GetKey(AppSettings::Recording_Store_Dir).toString();
    recDir = recDir + "/" + record_name;
    QDir dir;
    if(dir.exists(recDir))
    {
        LoggerDialog::get()->Error("StartRecording", QString("The record '%1'already exists").arg(record_name));
        return false;
    }

    dir.mkdir(recDir);
    _is_recording = true;
    _rec_path = recDir + "/";
    _rec_start_time = QDateTime::currentMSecsSinceEpoch();

    return true;
}

void Recorder::StopRecording()
{
    _is_recording = false;
}

bool Recorder::IsRecording()
{
    return _is_recording;
}

void Recorder::WriteData(QString track_name, QByteArray &data)
{
    if(!_is_recording)
        return;

    QString tract_path = _rec_path + track_name;
    QFile file(tract_path);

    if(!file.open(QIODevice::Append))
    {
        LoggerDialog::get()->Error("Recorder::WriteData", "Can't append to file: " + tract_path);
        return;
    }

    if(!writeDateTime(file))
    {
        LoggerDialog::get()->Error("Recorder::WriteData", "Can't write date to file: " + tract_path);
        file.close();
        return;
    }

    if(!writeChunkSize(file, data.length()))
    {
        LoggerDialog::get()->Error("Recorder::WriteData", "Can't write chunk size to file: " + tract_path);
        file.close();
        return;
    }

    if(file.write(data) == -1)
    {
        LoggerDialog::get()->Error("Recorder::WriteData", "Can't write data chunk to file: " + tract_path);
        file.close();
        return;
    }

    file.close();
    return;
}

bool Recorder::writeDateTime(QFile &file)
{
    qint64 date = QDateTime::currentMSecsSinceEpoch() - _rec_start_time;
    char * data = (char *)&date;
    return file.write(data, sizeof(qint64)) != -1;
}

bool Recorder::writeChunkSize(QFile &file, unsigned int length)
{
    char * data = (char *)&length;
    return file.write(data, 4) != -1;
}

