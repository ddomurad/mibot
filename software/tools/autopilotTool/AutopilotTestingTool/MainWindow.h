#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <RoverAutopilotClient.h>
#include <RoverSensorClient.h>
#include "OSMWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void onGPSMenu(OSMMenuItem);
    void newSensorData(RoverSensors);
    void apStateUpdate(AutopilotState);

    void on_checkBox_toggled(bool checked);
    void onTimer();
    void on_checkBox_2_toggled(bool checked);

    void on_toolButton_clicked();

private:
    Ui::MainWindow *ui;
    QImage image;
    QPixmap pixmap;

    RoverAutopilotClient * apClient;
    RoverSensorClient * scClient;

    QPointF _rover_gps_pos;
    QPointF _rover_goal;

    QPointF _fake_gps1;
    QPointF _fake_gps2;

    bool _ap_enabled;
    bool _fg_enabled;
    bool _fg_update;
    QTimer *_update_timer;

    void updateMarkers();
};

#endif // MAINWINDOW_H
