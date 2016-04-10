#ifndef RECORDER_H
#define RECORDER_H

#include <QString>
#include "Forms/LoggerDialog.h"
#include "Settings/AppSettings.h"

#include <QJsonObject>

/*
    File format:
    |DateTime|ChunkSieze|ChunkData|


    DateTime -> | MSecs Since Epoch |
                | int 64            | = 8 bytes

    ChunkSize-> |unsigned int| = 4 bytes

*/
class Recorder
{
public:
    ~Recorder();
    static Recorder * get();

    bool StartRecording(QString record_name);
    void StopRecording();

    bool IsRecording();
    void WriteData(QString track_name, QByteArray &data);
private:
    Recorder();

    bool _is_recording;
    QString _rec_path;

    bool writeDateTime(QFile &file);
    bool writeChunkSize(QFile &file, unsigned int length);
    qint64 _rec_start_time;
};

#endif // RECORDER_H
