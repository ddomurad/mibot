#ifndef ROVERCLIENTSPROVIDER_H
#define ROVERCLIENTSPROVIDER_H

#include "RoverSensorClient.h"
#include "RoverDriveClient.h"
#include "VideoServiceClient.h"
#include "RoverAutopilotClient.h"
#include "PictureClient.h"

class RoverClientsProvider
{
public:

    static RoverSensorClient * GetRoverSensorClient();
    static RoverDriveClient * GetRoverDriveClient();
    static VideoServiceClient * GetVideoServiceClient();
    static RoverAutopilotClient * GetRoverAutopilotClient();
    static PictureClient * GetPictureClient();

    ~RoverClientsProvider();
private:
    RoverClientsProvider();

    static RoverSensorClient * roverSensorClient;
    static RoverDriveClient * roverDriveClient;
    static VideoServiceClient * videoServerClient;
    static RoverAutopilotClient *roverAutopilotClient;
    static PictureClient * pictureClient;
};

#endif // ROVERCLIENTSPROVIDER_H

