#ifndef ARDUINOSENSORSNODEREADER_H
#define ARDUINOSENSORSNODEREADER_H

#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <mibAsyncWaitEvent.h>

#include "mibSensorsSettings.h"

namespace mibot
{

class ArduinoSensorsNodeReader : public QObject
{
    Q_OBJECT
public:
    const QString ACC_X_SENSOR_TAG = "acc_x";
    const QString ACC_Y_SENSOR_TAG = "acc_y";
    const QString ACC_Z_SENSOR_TAG = "acc_z";

    const QString MAG_X_SENSOR_TAG = "mag_x";
    const QString MAG_Y_SENSOR_TAG = "mag_y";
    const QString MAG_Z_SENSOR_TAG = "mag_z";

    const QString GPS_LON_1_SENSOR_TAG = "gps_lon_1";
    const QString GPS_LAT_1_SENSOR_TAG = "gps_lat_1";
    const QString GPS_LON_2_SENSOR_TAG = "gps_lon_2";
    const QString GPS_LAT_2_SENSOR_TAG = "gps_lat_2";
    const QString GPS_SPEED_SENSOR_TAG = "gps_speed";
    const QString GPS_DIR_SENSOR_TAG = "gps_dir";

    const int ACC_FLAG_ADDR = 1;
    const int MAG_FLAG_ADDR = 2;
    const int GPS_FLAG_ADDR = 3;
    const int ANALOG_BASE_FLAG_ADDR = 4;

    explicit ArduinoSensorsNodeReader(QObject *parent = 0);
    ~ArduinoSensorsNodeReader();

    bool IsReading();

    bool StartReader();
    void StopReader();

    QMap<QString, float> Readings();
signals:
    void StartReaderSignal();
    void StopReaderSignal();

private slots:
    void startReader();
    void stopReader();

    void onSerialPortRead();
    void onSettingsUpdateTimer();
private:
    SensorsSettings * _settings;
    AutoOpenAsyncWaitEvent _startEvent;
    bool _isReading;

    QSerialPort * _serialPort;
    QString _serialData;

    void processData(QString str);
    void processAcc(QString value);
    void processMag(QString value);
    void processAnalog(QString channel, QString value);
    void processGPS(QString value);

    void updateKey(QString key);

    void sendFlag(int addr, int value);

    QMap<QString, float> _readings;
    QMutex _readingMutex;

    int _last_analog_states[5];
    int _last_gps_state;
    int _last_acc_state;
    int _last_mag_state;

    QTimer *_settings_update_timer;
};
}

#endif // ARDUINOSENSORSNODEREADER_H
