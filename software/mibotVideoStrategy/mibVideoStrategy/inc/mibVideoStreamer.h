#ifndef STREAMER_H
#define STREAMER_H

#include <QtCore>
#include <pthread.h>
#include <vector>

namespace mibot
{

class VideoStreamer
{
public:
    VideoStreamer();
    ~VideoStreamer();

    QList<QString> Errors();
    void ClearErrors();

    bool StreamStart(QString addr, int port, QString video_device, int width, int height, int buffcount, QString format);
    void StreamStop();
    bool IsStreaming();
    int  Fps();

private:
    QList<QString> _errors;

    //video
    int _fd_video;
    std::vector<void*> _buffers_maps;
    int _fps;
    int _fps_cnt;
    timeval last_time;
    unsigned int _buf_length;
    //netowrk
    int _fd_socket;
    int _port;
    QString addr;

    //thread
    bool _shall_run;
    bool _is_running;
    pthread_t _thread;

    void thread_entry();
    static void* __thread_entry(void *);

    bool connectToServer(QString addr, int port);
    void disconnectFromServer();

    bool enableStream(QString video_device, int width, int height, unsigned int buffcount, QString format);
    void disableStream();

    bool startStreamThread();
    bool stopStreamThread();

    void pushError(QString error);
    void pushFps();

    int getFMTFromString(QString str);
    QString getStringFromFMT(int);
};

}
#endif // STREAMER_H
