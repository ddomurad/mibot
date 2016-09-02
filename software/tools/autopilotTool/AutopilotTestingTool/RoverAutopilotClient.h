#ifndef ROVERAUTOPILOTCLIENT_H
#define ROVERAUTOPILOTCLIENT_H

#include <QTimer>
#include "RoverClient.h"
#include "mibJSONProtocol.h"

class AutopilotState
{
public:
    double dist;
    double angle;
    int arived_at;
    QString drive_type;
};
class RoverAutopilotClient: public RoverClient
{
    Q_OBJECT
public:
    RoverAutopilotClient(QObject * parent = nullptr);
    ~RoverAutopilotClient();

    // RoverClient interface
public:
    bool Connect(QString addr, int port);
    void onData(QByteArray data);

    void SetAutopilot(QPointF target, int target_id, bool enable, bool fake_pos, QPointF fp1, QPointF fp2, int avoid_dist);
    void SetAutopilot(QPointF target, int target_id, bool enable, bool fake_pos, int avoid_dist);
    void SetAutopilot(QPointF target, int target_id, bool enable, int avoid_dist);
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
