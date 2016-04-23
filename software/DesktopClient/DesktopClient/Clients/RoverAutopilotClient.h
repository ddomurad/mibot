#ifndef ROVERAUTOPILOTCLIENT_H
#define ROVERAUTOPILOTCLIENT_H

#include <QTimer>
#include "./Clients/RoverClient.h"
#include "Other/JsInput.h"

class AutopilotState
{
public:
    double dist;
    double angle;
    int arived_at;
};
class RoverAutopilotClient: public RoverClient
{
    Q_OBJECT
public:
    RoverAutopilotClient(QObject * parent = nullptr);
    ~RoverAutopilotClient();

    // RoverClient interface
public:
    bool Connect();
    void onData(QByteArray data);

    void SetAutopilot(QPointF target, int target_id, bool enable);
    void SetAutopilot(bool enable);

signals:
    void StateUpdate(AutopilotState);

protected:
    void onConnection();
    QString clientName();

private:
    void processObj(QJsonObject & obj);
    mibot::JSONProtocol protocol;

    void fixIfJsonIsCorrupted();

};

#endif // ROVERAUTOPILOTCLIENT_H
