#include <linux/videodev2.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>

#include "inc/mibVideoStreamer.h"
#include <mibLogger.h>

using namespace mibot;

VideoStreamer::VideoStreamer():
    _fps(0),
    _shall_run(false),
    _is_running(false),
    _thread(0)
{
    gettimeofday(&last_time, nullptr);
}

VideoStreamer::~VideoStreamer()
{

}

bool VideoStreamer::IsStreaming()
{
    return _is_running;
}

int VideoStreamer::Fps()
{
    return _fps;
}

void VideoStreamer::thread_entry()
{
    _is_running = true;
    while(_shall_run)
    {
        v4l2_buffer buffer;
        memset(&buffer, 0, sizeof(v4l2_buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;

        if(ioctl(_fd_video, VIDIOC_DQBUF, &buffer) == -1)
        {
            pushError("Can't dequeue buffer!");
            _is_running = false;
            return;
        }

        if(write(_fd_socket, _buffers_maps[buffer.index],buffer.bytesused) == -1)
        {
            pushError("Can't send frame data!");
            _is_running = false;
            return;
        }

        if( ioctl(_fd_video, VIDIOC_QBUF, &buffer) == -1)
        {
            pushError("Can't queue buffer!");
            _is_running = false;
            return;
        }

        pushFps();
    }

    _is_running = false;
}

void VideoStreamer::StreamStop()
{
    if(!stopStreamThread())
        pushError("Thread closed can't join.");

    disableStream();
    disconnectFromServer();
}

bool VideoStreamer::StreamStart(QString addr, int port, QString video_device, int width, int height, int buffcount, QString format)
{
    if(!connectToServer(addr,port)) return false;
    if(enableStream(video_device, width, height, buffcount, format))
    {
        if(!startStreamThread())
        {
            disableStream();
            disconnectFromServer();
            return false;
        }
    }
    else
    {
        disconnectFromServer();
        return false;
    }

    return true;
}

void VideoStreamer::ClearErrors()
{
    _errors.clear();
}

QList<QString> VideoStreamer::Errors()
{
    return _errors;
}

void *VideoStreamer::__thread_entry(void * self)
{
    VideoStreamer * selfObj = (VideoStreamer*)self;
    selfObj->thread_entry();
    return 0;
}

bool VideoStreamer::connectToServer(QString addr, int port)
{
    if((_fd_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        pushError("Can't create socket!");
        return false;
    }

    struct hostent * server = gethostbyname( addr.toStdString().c_str() );
    if(server == nullptr)
    {
        pushError( QString("Can't find host: %1").arg(addr) );
        close(_fd_socket);
        return false;
    }

    struct sockaddr_in serv_addr;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(port);

    if (connect(_fd_socket,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        pushError(QString("Can't connect to: %1:%2").arg(addr).arg(port));
        close(_fd_socket);
        return false;
    }

    return true;
}

void VideoStreamer::disconnectFromServer()
{
    close(_fd_socket);
}

bool VideoStreamer::enableStream(QString video_device, int width, int height, unsigned int buffcount, QString fmt)
{
    if((_fd_video = open(video_device.toStdString().c_str(), O_RDWR)) == -1)
    {
        pushError(QString("Can't open device: %1").arg(video_device));
        return false;
    }

    v4l2_format format;
    memset(&format, 0, sizeof(v4l2_format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = width;
    format.fmt.pix.height = height;
    format.fmt.pix.pixelformat = getFMTFromString(fmt);
    format.fmt.pix.field = V4L2_FIELD_ANY;

    v4l2_requestbuffers reqbuf;
    memset(&reqbuf, 0, sizeof(v4l2_requestbuffers));

    reqbuf.count = buffcount;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if( ioctl(_fd_video, VIDIOC_S_FMT, &format) == -1)
    {
        pushError(QString("Can't set frame format. (w: %1, h: %2, fmt: %3)").arg(width).arg(height).arg(fmt));
        close(_fd_video);
        return false;
    }

    if( ioctl(_fd_video, VIDIOC_REQBUFS, &reqbuf) == -1)
    {
        pushError(QString("Can't request %1 buffers.").arg(buffcount));
        close(_fd_video);
        return false;
    }

    if(reqbuf.count != buffcount)
    {
        pushError(QString("Can't request %1 buffers, instead %2 buffers has been requested.").arg(buffcount).arg(reqbuf.count));
        close(_fd_video);
        return false;
    }

    for(unsigned int i=0; i<buffcount; i++)
    {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if(ioctl(_fd_video, VIDIOC_QUERYBUF, &buf ) == -1)
        {
            pushError("Can't reqeust buffer");
            close(_fd_video);
            return false;
        }

        void * map = mmap(NULL, buf.length,
                          PROT_READ | PROT_WRITE,
                          MAP_SHARED, _fd_video, buf.m.offset);

        _buf_length = buf.length;

        if( map == MAP_FAILED)
        {
            pushError("Can't map buffers memory");
            close(_fd_video);
            return false;
        }

        _buffers_maps.push_back(map);
    }

    //start streaming

    for( unsigned int i=0; i<buffcount; i++)
    {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if( ioctl(_fd_video, VIDIOC_QBUF, &buf) == -1)
        {
            pushError("Can't que buffer.");
            close(_fd_video);
            return false;
        }
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( ioctl(_fd_video, VIDIOC_STREAMON, &type) == -1 )
    {
        pushError("Can't enable steam");
        close(_fd_video);
        return false;
    }

    return true;
}

void VideoStreamer::disableStream()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(_fd_video, VIDIOC_STREAMOFF, &type);

    for(unsigned int i=0;i<_buffers_maps.size(); i++)
        munmap(_buffers_maps.at(i), _buf_length);

    _buffers_maps.clear();

    close(_fd_video);
}

bool VideoStreamer::startStreamThread()
{
    if(_thread != 0)
    {
        pushError("Thread is not stoped!");
        return false;
    }

    _shall_run = true;
    if(pthread_create(&_thread,0, VideoStreamer::__thread_entry, this ) < 0)
    {
        pushError("Can't create thread.");
        return false;
    }

    return true;
}

bool VideoStreamer::stopStreamThread()
{
    if(_thread == 0) return true;

    _shall_run = false;
    void * ret;
    timespec time_spec;
    clock_gettime(CLOCK_REALTIME,&time_spec);
    time_spec.tv_sec += 5;

    bool r = pthread_timedjoin_np(_thread,&ret,&time_spec) == 0;
    _thread = 0;
    return r;
}

void VideoStreamer::pushError(QString error)
{
    if(errno != 0)
    {
        _errors.append( QString("ERROR: [%1]; ERRNO: [%2]").arg(error).arg(strerror(errno)));
    }else
    {
        _errors.append( QString("ERROR: [%1]").arg(error));
    }
}

void VideoStreamer::pushFps()
{
    timeval curr_time;
    gettimeofday(&curr_time, nullptr);

    qreal elapsedTime;
    elapsedTime = (curr_time.tv_sec - last_time.tv_sec) * 1000.0;
    elapsedTime += (curr_time.tv_usec - last_time.tv_usec) / 1000.0;

    if(elapsedTime >= 1000)
    {
        _fps =  _fps_cnt;
        _fps_cnt = 0;

        last_time = curr_time;
    }else
    {
        _fps_cnt++;
    }
}

int VideoStreamer::getFMTFromString(QString str)
{
    if(str == "h264") return V4L2_PIX_FMT_H264;
    if(str == "mjpeg") return V4L2_PIX_FMT_MJPEG;
    if(str == "yuv420m") return V4L2_PIX_FMT_YUV420M;

    return 0;
}

QString VideoStreamer::getStringFromFMT(int fmt)
{
    switch(fmt)
    {
        case V4L2_PIX_FMT_H264: return "h264";
        case V4L2_PIX_FMT_MJPEG: return "mjpeg";
        case V4L2_PIX_FMT_YUV420M: return "yuv420m";
    default: return "unknown";
    }
}
