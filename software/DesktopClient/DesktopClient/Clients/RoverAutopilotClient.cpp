#include "./Clients/RoverAutopilotClient.h"
#include "./Forms/LoggerDialog.h"
#include "./Settings/AppSettings.h"

RoverAutopilotClient::RoverAutopilotClient(QObject *parent) :
    RoverClient(parent)
{}

RoverAutopilotClient::~RoverAutopilotClient()
{}

bool RoverAutopilotClient::Connect()
{
  return _startConnecting(AppSettings::GetKey(AppSettings::Connection_Autopilot_Service).toString());
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

    emit StateUpdate(state);
}

void RoverAutopilotClient::fixIfJsonIsCorrupted()
{
    if(protocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("JSON Parser", "StatusStrategy JSONProtocol handler contains corrupted json data. The handler will try to remove corrupted data.");
        protocol.RemoveCoruptedData();
    }
}

