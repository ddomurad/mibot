#ifndef TESTER_H
#define TESTER_H

#include <QObject>
#include <mibAutopilot.h>

class Tester : public QObject
{
    Q_OBJECT
public:
    explicit Tester(QObject *parent = 0);
    ~Tester();

signals:
    void TestDone();

public slots:
    void Test();

private:
    mibot::Autopilot * autoPilot;
    mibot::Connection * connection;
    mibot::AutopilotSettings *autopilotSettings;
    mibot::DriveConfigSettings *driveSettings;
    mibot::GPSSensor *gpsSensor;
};

#endif // TESTER_H
