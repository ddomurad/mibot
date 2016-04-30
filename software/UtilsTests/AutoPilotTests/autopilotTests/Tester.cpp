#include "Tester.h"

Tester::Tester(QObject *parent) : QObject(parent)
{
    autopilotSettings = new mibot::AutopilotSettings("autopilot");
    driveSettings = new mibot::DriveConfigSettings("drive");
    gpsSensor = mibot::GPSSensor::get();

    driveSettings->alias->value = "drive";
    driveSettings->emergencyBreakTimeout->value = 1000;

    driveSettings->leftAPin->value = 1;
    driveSettings->leftBPin->value = 3;
    driveSettings->leftPwmPin->value = 2;

    driveSettings->rightAPin->value = 11;
    driveSettings->rightBPin->value = 13;
    driveSettings->rightPwmPin->value = 12;

    driveSettings->updateRatio->value = 100;
    driveSettings->useFakeGPIO->value = true;

    autopilotSettings->angleTreshold->value = 30;
    autopilotSettings->baseTimeFactor->value = 1.0;
    autopilotSettings->driveSettings->value = "drive";
    autopilotSettings->dynamicTimeFactor->value = false;
    autopilotSettings->forwardDriveTime->value = 2000;
    autopilotSettings->maxDistance->value = 0.01;
    autopilotSettings->statuTimerInterval->value = 500;
    autopilotSettings->watchDogTimerInterval->value = 1000;

    connection = new mibot::Connection();
    autoPilot = new mibot::Autopilot(connection);
    autoPilot->SetupTest(autopilotSettings, driveSettings, gpsSensor);


}

Tester::~Tester()
{
    delete autoPilot;
}

void Tester::Test()
{
    autoPilot->onDriverTimer();
    emit TestDone();
}

