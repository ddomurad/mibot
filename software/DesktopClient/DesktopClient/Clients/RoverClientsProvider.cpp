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

RoverAutopilotClient *RoverClientsProvider::GetRoverAutopilotClient()
{
    if(roverAutopilotClient == nullptr)
        roverAutopilotClient = new RoverAutopilotClient();

    return roverAutopilotClient;
}

PictureClient *RoverClientsProvider::GetPictureClient()
{
    if(pictureClient == nullptr)
        pictureClient = new PictureClient();

    return pictureClient;
}

RoverClientsProvider::~RoverClientsProvider()
{
}

RoverSensorClient * RoverClientsProvider::roverSensorClient = nullptr;
RoverDriveClient * RoverClientsProvider::roverDriveClient = nullptr;
VideoServiceClient * RoverClientsProvider::videoServerClient = nullptr;
RoverAutopilotClient * RoverClientsProvider::roverAutopilotClient = nullptr;
PictureClient * RoverClientsProvider::pictureClient = nullptr;
