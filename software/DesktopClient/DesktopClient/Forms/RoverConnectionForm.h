#ifndef ROVERCONNECTIONFORM_H
#define ROVERCONNECTIONFORM_H

#include "ReferenceCounter.h"
#include <QWidget>

namespace Ui {
class RoverConnectionForm;
}

class RoverConnectionForm : public QWidget, public ReferenceCounter<RoverConnectionForm>
{
    Q_OBJECT

public:
    explicit RoverConnectionForm(QWidget *parent = 0);
    ~RoverConnectionForm();

private slots:
    void on_update();

    void on_pushButton_disconnect_all_clicked();
    void on_pushButton_connect_all_clicked();
    void on_pushButton_disconnect_video_clicked();
    void on_pushButton_connect_video_clicked();
    void on_pushButton_connect_drive_clicked();
    void on_pushButton_disconnect_driver_clicked();
    void on_pushButton_con_sensors_clicked();
    void on_pushButton_disconnect_sensors_clicked();

private:
    Ui::RoverConnectionForm *ui;
    QTimer *_updateTimer;
};

#endif // ROVERCONNECTIONFORM_H
