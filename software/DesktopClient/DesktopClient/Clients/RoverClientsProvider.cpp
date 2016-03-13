#include "RoverClientsProvider.h"

RoverClientsProvider::RoverClientsProvider()
{

}

RoverSensorClient *RoverClientsProvider::GetRoverSensorClient()
{
    if(roverSensorClient == nullptr)
        roverSensorClient = new RoverSensorClient();

    return roverSensorClient;
}

RoverDriveClient *RoverClientsProvider::GetRoverDriveClient()
{
    if(roverDriveClient == nullptr)
        roverDriveClient = new RoverDriveClient();

    return roverDriveClient;
}

VideoServiceClient *RoverClientsProvider::GetVideoServiceClient()
{
    if(videoServerClient == nullptr)
        videoServerClient = new VideoServiceClient();

    return videoServerClient;
}

RoverClientsProvider::~RoverClientsProvider()
{
}

RoverSensorClient * RoverClientsProvider::roverSensorClient = nullptr;
RoverDriveClient * RoverClientsProvider::roverDriveClient = nullptr;
VideoServiceClient * RoverClientsProvider::videoServerClient = nullptr;
