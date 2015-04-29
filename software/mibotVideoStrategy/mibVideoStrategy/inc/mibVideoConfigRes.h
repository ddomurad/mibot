#ifndef VIDEOCONFIGRES_H
#define VIDEOCONFIGRES_H

#include <mibAbstractResource.h>

namespace mibot
{

class VideoConfigRes : public AbstractResource
{
public:
    VideoConfigRes();
    ~VideoConfigRes();

    QString Alias();
    QString Device();

    int FpsPrescaler();

    int FrameWidth();
    int FrameHeight();
};

}
#endif // VIDEOCONFIGRES_H
