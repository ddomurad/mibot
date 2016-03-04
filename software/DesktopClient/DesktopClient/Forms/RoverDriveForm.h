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
    void on_checkBox_toggled(bool checked);
    void onUpdateDrive();
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
};

#endif // ROVERDRIVEFORM_H
