#ifndef ROVERCLIENTSPROVIDER_H
#define ROVERCLIENTSPROVIDER_H

#include "RoverSensorClient.h"
#include "RoverDriveClient.h"
#include "VideoServiceClient.h"

class RoverClientsProvider
{
public:

    static RoverSensorClient * GetRoverSensorClient();
    static RoverDriveClient * GetRoverDriveClient();
    static VideoServiceClient * GetVideoServiceClient();

    ~RoverClientsProvider();
private:
    RoverClientsProvider();

    static RoverSensorClient * roverSensorClient;
    static RoverDriveClient * roverDriveClient;
    static VideoServiceClient * videoServerClient;
};

#endif // ROVERCLIENTSPROVIDER_H

