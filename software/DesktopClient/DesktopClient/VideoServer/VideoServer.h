#ifndef VIDEOSERVER_H
#define VIDEOSERVER_H

#include "./Settings/AppSettings.h"

class VideoServer
{
public:
    ~VideoServer();
    VideoServer * get();
private:
    VideoServer();
};

#endif // VIDEOSERVER_H
