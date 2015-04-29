#ifndef V4L2WRAPPER_H
#define V4L2WRAPPER_H

#include <QString>
#include <linux/videodev2.h>

namespace mibot
{

class BufferPtr
{
public:
    void * ptr;
    unsigned int len;
};

class V4L2Wrapper
{
public:
    V4L2Wrapper();
    ~V4L2Wrapper();

    enum class VideoFormat { H264, MJPEG, YUYV };

    bool Open(QString dev,
              int buffers_count,
              int w, int h, bool blocking = true,
              VideoFormat vformat = VideoFormat::YUYV);

    void Close();

    bool  Start();
    void  Stop();

    char * GrabNext(unsigned int *size);


private:
    int _fd;
    unsigned int _buff_cnt;
    unsigned int _width;
    unsigned int _height;

    bool _is_opened;
    bool _is_started;
    BufferPtr * _buffer_ptrs;
    unsigned int getFormat(VideoFormat format);
    QString getFormatString(VideoFormat format);

    char * _frame_data;
};

}

#endif // V4L2WRAPPER_H
