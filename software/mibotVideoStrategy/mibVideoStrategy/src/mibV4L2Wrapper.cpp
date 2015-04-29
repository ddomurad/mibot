#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/videodev2.h>
#include <jpeglib.h>

#include <mibLogger.h>
#include "inc/mibV4L2Wrapper.h"

using namespace mibot;

V4L2Wrapper::V4L2Wrapper() :
    _is_opened(false),
    _is_started(false),
    _buffer_ptrs(0)

{}

V4L2Wrapper::~V4L2Wrapper()
{
    if(_buffer_ptrs != 0)
        delete[] _buffer_ptrs;
}

bool V4L2Wrapper::Open(QString dev, int buffers_count, int w, int h,
                       bool blocking ,
                       V4L2Wrapper::VideoFormat vformat)
{
    LOG_INFO( QString("Opening video device: '%1'' [buff: %2, w: %3, h: %4]")
              .arg(dev).arg(buffers_count).arg(w).arg(h));

    int open_flags = O_RDWR | ( blocking ? 0x00 : O_NONBLOCK);
    _fd = open(dev.toStdString().c_str(), open_flags);

    if( _fd == -1)
    {
        LOG_ERROR( QString("Can't open video device: '%1'").arg(dev));
        _is_opened = false;
        return false;
    }

    _buff_cnt = buffers_count;
    _width = w;
    _height = h;

    v4l2_format format;
    memset(&format, 0, sizeof(v4l2_format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.width = _width;
    format.fmt.pix.height = _height;
    format.fmt.pix.pixelformat = getFormat(vformat);
    format.fmt.pix.field = V4L2_FIELD_ANY;

    v4l2_requestbuffers reqbuf;
    memset(&reqbuf, 0, sizeof(v4l2_requestbuffers));

    reqbuf.count = _buff_cnt;
    reqbuf.memory = V4L2_MEMORY_MMAP;
    reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    int ret = ioctl(_fd, VIDIOC_S_FMT, &format);
    if(ret == -1)
    {
        LOG_ERROR( QString("Can't set format. (w: %1, h: %2, bufs: %3, format: %4) [%5]")
                   .arg(_width).arg(_height).arg(_buff_cnt)
                   .arg(getFormatString(vformat))
                   .arg(QString(strerror(errno))) );

        close(_fd);
        _is_opened = false;
        return false;
    }

    if(format.fmt.pix.width != _width
            || format.fmt.pix.height != _height
            || format.fmt.pix.pixelformat != getFormat(vformat))
    {
        LOG_ERROR( QString("Can't set format. (w: %1, h: %2, bufs: %3, format: %4)")
                   .arg(_width).arg(_height).arg(_buff_cnt)
                   .arg(getFormatString(vformat)));

        close(_fd);
        _is_opened = false;
        return false;
    }

    ret = ioctl(_fd, VIDIOC_REQBUFS, &reqbuf);

    if(ret == -1)
    {
        LOG_ERROR( QString("Can't request buffers [%1]").arg(QString(strerror(errno))));
        close(_fd);
        _is_opened = false;
        return false;
    }

    if(_buff_cnt > reqbuf.count)
    {
        LOG_ERROR( QString("Can't request buffers '_buff_cnt > reqbuf.count; [%1 != %2]")
                   .arg(_buff_cnt).arg(reqbuf.count));

        close(_fd);
        _is_opened = false;
        return false;
    }

    _buffer_ptrs = new BufferPtr[_buff_cnt];

    for( unsigned int i=0; i<_buff_cnt; i++)
    {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        ret = ioctl(_fd, VIDIOC_QUERYBUF, &buf );

        if(ret == -1)
        {
            LOG_ERROR(QString("Can't query buffer: i: %1 [%2]")
                      .arg(i).arg( QString(strerror(errno)) ));

            close(_fd);
            _is_opened = false;
            return false;
        }

        _buffer_ptrs[i].len = (unsigned int)buf.length;
        _buffer_ptrs[i].ptr = mmap(NULL, buf.length,
                                   PROT_READ | PROT_WRITE,
                                   MAP_SHARED, _fd, buf.m.offset);

        if( _buffer_ptrs[i].ptr == MAP_FAILED )
        {
            LOG_ERROR(QString("Can't query buffer (map_failed): i: %1 [%2]")
                      .arg(i).arg( QString(strerror(errno)) ));

            close(_fd);
            _is_opened = false;
            return false;
        }
    }

    _is_opened = true;
    return true;
}

void V4L2Wrapper::Close()
{
    if(_buffer_ptrs != 0)
    {
        delete[] _buffer_ptrs;
        _buffer_ptrs = 0;
    }

    close(_fd);
    _is_opened = false;
}

bool V4L2Wrapper::Start()
{
    if(_is_started == true || _is_opened == false) return true;

    for( unsigned int i=0; i<_buff_cnt; i++)
    {
        v4l2_buffer buf;
        memset(&buf, 0, sizeof(v4l2_buffer));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if( ioctl(_fd, VIDIOC_QBUF, &buf) == -1)
        {
            LOG_ERROR(QString("Can't start queue buffer: i: %1 [%2]")
                      .arg(i).arg( QString(strerror(errno)) ));

            _is_started = false;
            return false;
        }
    }

    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( ioctl(_fd, VIDIOC_STREAMON, &type) == -1 )
    {
        LOG_ERROR(QString("Can't enable streaming. [%2]")
                  .arg( QString(strerror(errno)) ));

        _is_started = false;
        return false;
    }

    _grab_index = 0;
    _is_started = true;
    return true;
}

void V4L2Wrapper::Stop()
{
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if( ioctl(_fd, VIDIOC_STREAMOFF, &type) == -1 )
    {
        LOG_ERROR(QString("Can't disable streaming. [%2]")
                  .arg( QString(strerror(errno)) ));

        return;
    }

    _is_started = false;
}

char *V4L2Wrapper::GrabNext(unsigned int *size)
{
    if(_is_opened == false || _is_started == false) return 0;

    v4l2_buffer buffer;
    memset(&buffer, 0, sizeof(v4l2_buffer));
    buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buffer.memory = V4L2_MEMORY_MMAP;
    buffer.index = _grab_index;

    if(ioctl(_fd, VIDIOC_DQBUF, &buffer) == -1)
        return 0;

    char * ptr = (char *)_buffer_ptrs[_grab_index].ptr;
    *size = _buffer_ptrs[_grab_index].len;

    if( ioctl(_fd, VIDIOC_QBUF, &buffer) == -1)
    {
        LOG_WARNING(QString("Can't queue buffer: _grab_index: %1 [%2]")
                  .arg(_grab_index).arg( QString(strerror(errno)) ));

        return 0;
    }

    _grab_index++;
    _grab_index %= _buff_cnt;
    return ptr ;
}

unsigned int V4L2Wrapper::getFormat(V4L2Wrapper::VideoFormat format)
{
    switch (format)
    {
    case VideoFormat::H264:
        return V4L2_PIX_FMT_H264;
    case VideoFormat::MJPEG:
        return V4L2_PIX_FMT_MJPEG;
    case VideoFormat::YUYV:
        return V4L2_PIX_FMT_YUYV;
    }

    return 0;
}

QString V4L2Wrapper::getFormatString(V4L2Wrapper::VideoFormat format)
{
    switch (format)
    {
    case VideoFormat::H264:
        return "V4L2_PIX_FMT_H264";
    case VideoFormat::MJPEG:
        return "V4L2_PIX_FMT_MJPEG";
    case VideoFormat::YUYV:
        return "V4L2_PIX_FMT_YUYV";
    }

    return "unknown";
}
