#ifndef ARDUINOSENSORSNODEREADER_H
#define ARDUINOSENSORSNODEREADER_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <mibAsyncWaitEvent.h>

#include "mibSensorsSettings.h"

namespace mibot
{

class ArduinoReadings
{
   public:
    double acc[3];
    double mag[3];

    float analogValue[10];
    bool isAnalogValue[10];

    int us;
};

class ArduinoSensorsNodeReader : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoSensorsNodeReader(QObject *parent = 0);
    ~ArduinoSensorsNodeReader();

    bool IsReading();

    bool StartReader();
    void StopReader();

    ArduinoReadings Readings();

    void SendCommand(char cmd, char value);
    bool CalibrateAcc(double new_acc_scale);
signals:
    void StartReaderSignal();
    void StopReaderSignal();
    void SetPiezo(bool state);

private slots:
    void onSetPiezo(bool state);
    void startReader();
    void stopReader();

    void onSerialPortRead();
    void onSettingsUpdateTimer();
    void onResetMotorLock();

private:
    SensorsSettings * _settings;
    AutoOpenAsyncWaitEvent _startEvent;
    bool _isReading;

    QSerialPort * _serialPort;
    QString _serialData;

    void processData(QString str);
    void processUs(QString value);
    void processAcc(QString value);
    void processMag(QString value);
    void processAnalog(QCharRef channel, QString value);

    ArduinoReadings _readings;
    QMutex _readingMutex;
    QMutex _sendingMutex;

    int _last_analog_states[10];
    int _last_acc_state;
    int _last_mag_state;

    QTimer *_settings_update_timer;
    QTimer *_motor_lock_timer;
};
}

#endif // ARDUINOSENSORSNODEREADER_H
