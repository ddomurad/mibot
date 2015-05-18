#include <mibLogger.h>
#include "inc/mibVideoProcessRunner.h"

using namespace mibot;

VideoProcessRunner::VideoProcessRunner(QObject *parent) : QObject(parent)
{
    _process = new QProcess(this);
    connect(_process, SIGNAL(finished(int)),this ,SIGNAL(OnStop(int)));
    connect(_process, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
    connect(_process, SIGNAL(started()), this, SIGNAL(OnStart()));
}

VideoProcessRunner::~VideoProcessRunner()
{

}

bool VideoProcessRunner::Start(int w, int h, int fps, int port)
{
    QString arg = "raspivid -w %1 -h %2 -fps %3 -t 0 -o - | socat tcp-l:%4,reuseaddr -";
    QStringList args;
    args << arg.arg(w).arg(h).arg(fps).arg(port);

    _process->setArguments( args );
    _process->setProgram("/usr/local/bin/mi_bot/mibSystemExecute");

    _process->start();
    if(!_process->waitForStarted(1000))
    {
        return false;
    }

    return true;
}

void VideoProcessRunner::Stop()
{
    //_process->terminate();
    system("pkill -9 raspivid");
    if(!_process->waitForFinished(1000))
    {
        LOG_ERROR("Problem with killingraspivid stream command");
    }
}

void VideoProcessRunner::OnReadyRead()
{
    QString data = _process->readAll();
    emit OnRead( data );
}

