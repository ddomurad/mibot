#include "LoggerDialog.h"
#include "Settings/AppSettings.h"
#include "RoverDriveForm.h"
#include "ui_RoverDriveForm.h"

RoverDriveForm::RoverDriveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoverDriveForm),
    _last_horn_state(false)
{
    ui->setupUi(this);

    _timer = new QTimer(this);
    _js = new JsInput(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onUpdateDrive()));
}

RoverDriveForm::~RoverDriveForm()
{
    delete ui;
}

void RoverDriveForm::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        UpdateSettings();
        _timer->start(_update_ms);
        _js->Start(_device_path);
    }else
    {
        _timer->stop();
        _js->Stop();
    }
}

void RoverDriveForm::onUpdateDrive()
{
    if(!RoverClientsProvider::GetRoverDriveClient()->IsConnected())
    {
        LoggerDialog::get()->Warning("Send drive command", "Drive client not connected.");
        ui->checkBox->setChecked(false);
        return;
    }

    int drive_ax_val = _js->axis[_drive_axis] * (_swap_drive ? -1 : 1);
    int turn_ax_val = _js->axis[_turn_axis]  * (_swap_turn ? -1 : 1);
    bool turbo_state = _js->btn[_turbo_btn];
    bool brake_state = _js->btn[_brake_btn];
    bool horn_state = _js->btn[_horn_btn];

    RoverClientsProvider::GetRoverDriveClient()->
            SendDriveCommand(drive_ax_val, turn_ax_val, brake_state, turbo_state);

    if(horn_state != _last_horn_state)
    {
        _last_horn_state = horn_state;
        RoverClientsProvider::GetRoverSensorClient()->SetSoundSignal(horn_state);
    }
}

void RoverDriveForm::UpdateSettings()
{
    _update_ms = AppSettings::GetKey(AppSettings::JS_Send_Ms).toInt();

    _device_path = AppSettings::GetKey(AppSettings::JS_Device_Path).toString();

    _swap_drive = AppSettings::GetKey(AppSettings::JS_Swap_Drive).toBool();
    _swap_turn = AppSettings::GetKey(AppSettings::JS_Swap_Trun).toBool();

    _drive_axis = AppSettings::GetKey(AppSettings::JS_Drive_Axis).toInt();
    _turn_axis = AppSettings::GetKey(AppSettings::JS_Turn_Axis).toInt();

    _turbo_btn = AppSettings::GetKey(AppSettings::JS_Turbo_Btn).toInt();
    _brake_btn = AppSettings::GetKey(AppSettings::JS_Brake_Btn).toInt();
    _horn_btn = AppSettings::GetKey(AppSettings::JS_Horn_Btn).toInt();
}
