#ifndef PROCESSRUNNER_H
#define PROCESSRUNNER_H

#include <QObject>
#include <QProcess>

namespace mibot
{
class VideoProcessRunner : public QObject
{
    Q_OBJECT
public:
    explicit VideoProcessRunner(QObject *parent = 0);
    ~VideoProcessRunner();

signals:
    void OnStart();
    void OnStop(int c);
    void OnRead(QString str);

public slots:
    bool Start(int w, int h, int fps, int port);
    void Stop();

    void OnReadyRead();
private:
    QProcess * _process;
};

}

#endif // PROCESSRUNNER_H
