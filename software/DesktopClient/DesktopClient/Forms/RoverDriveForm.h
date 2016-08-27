#ifndef ROVERDRIVEFORM_H
#define ROVERDRIVEFORM_H

#include "Other/JsInput.h"
#include <QTimer>
#include <QWidget>
#include "Clients/RoverClientsProvider.h"
#include "ReferenceCounter.h"

namespace Ui {
class RoverDriveForm;
}

class RoverDriveForm : public QWidget, public ReferenceCounter<RoverDriveForm>
{
    Q_OBJECT

public:
    explicit RoverDriveForm(QWidget *parent = 0);
    ~RoverDriveForm();

private slots:
    void onUpdateDrive();

    void on_pushButton_next_point_clicked();

    void on_pushButton_prev_point_clicked();

    void on_pushButton_auto_clicked();

    void on_comboBox_drive_type_currentTextChanged(const QString &arg1);

    void on_comboBox_track_currentIndexChanged(const QString &arg1);

    void on_spinBox_active_point_valueChanged(int arg1);

    void on_comboBox_drive_type_currentIndexChanged(const QString &arg1);

    void onAutopilotUpdate(AutopilotState state);
    void onSensorsUpdate(class RoverSensors readings);
    void on_checkBox_drive_toggled(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_checkBox_brake_on_us_toggled(bool checked);
    void on_spinBox_dist_valueChanged(int arg1);

private:
    void UpdateSettings();
    Ui::RoverDriveForm *ui;

    QString _device_path;
    int _drive_axis;
    int _turn_axis;

    int _brake_btn;
    int _turbo_btn;
    int _horn_btn;

    bool _swap_drive;
    bool _swap_turn;

    int _update_ms;

    QTimer * _timer;
    JsInput * _js;

    bool _last_horn_state;

    void updateStats();

    void updateManual();
    void updateAutopilot();
    void setAutopilotBreak(bool state);

    bool _run_on_autopilot;
    bool _autopilot_break;

    bool _us_brake;
    int _us_brake_dist;
    int _us_brake_cnt;
};

#endif // ROVERDRIVEFORM_H
