#include "RoverAutopilotClient.h"

RoverAutopilotClient::RoverAutopilotClient(QObject *parent) :
    RoverClient(parent)
{}

RoverAutopilotClient::~RoverAutopilotClient()
{}

bool RoverAutopilotClient::Connect(QString addr, int port)
{
  return _startConnecting(addr, port);
}

void RoverAutopilotClient::onData(QByteArray data)
{
    protocol.PushData(data);
    fixIfJsonIsCorrupted();

    while(protocol.ContainsValidJSONObject())
    {
        QJsonObject obj = protocol.GetPendingObject();
        processObj(obj);
        fixIfJsonIsCorrupted();
    }
}

void RoverAutopilotClient::SetAutopilot(QPointF target, int target_id, bool enable, bool fake_pos, QPointF fp1, QPointF fp2)
{
    SendData(QString("{\"autopilot_enabled\":%1,\"gps_pos\":[%2,%3],\"point_id\":%4,\"fake_pos\":%5,\"fp1\":[%6,%7],\"fp2\":[%8,%9]}")
             .arg(enable ? "true" : "false")
             .arg(target.x())
             .arg(target.y())
             .arg(target_id)
             .arg(fake_pos ? "1", "0")
             .arg(fp1.x())
             .arg(fp1.y())
             .arg(fp2.x())
             .arg(fp2.y()));
}

void RoverAutopilotClient::SetAutopilot(QPointF target, int target_id, bool enable, bool fake_pos)
{
    SendData(QString("{\"autopilot_enabled\":%1,\"gps_pos\":[%2,%3],\"point_id\":%4,\"fake_pos\":%5}")
             .arg(enable ? "true" : "false")
             .arg(target.x())
             .arg(target.y())
             .arg(target_id)
             .arg(fake_pos ? "1", "0"));
}

void RoverAutopilotClient::SetAutopilot(QPointF target, int target_id, bool enable)
{
    SendData(QString("{\"autopilot_enabled\":%1,\"gps_pos\":[%2,%3],\"point_id\":%4}")
             .arg(enable ? "true" : "false")
             .arg(target.x())
             .arg(target.y())
             .arg(target_id));
}

void RoverAutopilotClient::SetAutopilot(bool enable)
{
    if(enable)
        SendData("{\"autopilot_enabled\":true}");
    else
        SendData("{\"autopilot_enabled\":false}");
}

void RoverAutopilotClient::onConnection()
{

}

QString RoverAutopilotClient::clientName()
{
    return "RoverAutopilotClient";
}

void RoverAutopilotClient::processObj(QJsonObject &obj)
{
    AutopilotState state;
    state.dist = 0;
    state.angle = 0;
    state.arived_at = -1;

    if(obj["distance"].isDouble())
        state.dist = obj["distance"].toDouble();

    if(obj["angle"].isDouble())
        state.angle = obj["angle"].toDouble();

    if(obj["arrived_at"].isDouble())
        state.arived_at = obj["arrived_at"].toDouble();

    if(obj["drive_type"].isString())
        state.drive_type = obj["drive_type"].toString();

    if(obj["no_gps"].isDouble())
        if(obj["no_gps"].toDouble() == 1)
            printf("Autopilot: NO GPS!\n");

    emit StateUpdate(state);
}

void RoverAutopilotClient::fixIfJsonIsCorrupted()
{
    if(protocol.IsDataCorupted())
    {
        printf("JSON Parser: StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.\n");
        protocol.RemoveCoruptedData();
    }
}

