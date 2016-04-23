#include <QDebug>
#include "LoggerDialog.h"
#include "Settings/AppSettings.h"
#include "RoverDriveForm.h"
#include "ui_RoverDriveForm.h"
#include "Other/TrackProvider.h"

RoverDriveForm::RoverDriveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoverDriveForm),
    _last_horn_state(false)
{
    ui->setupUi(this);

    _timer = new QTimer(this);
    _js = new JsInput(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onUpdateDrive()));

    TrackProvider::LoadRoutesFromFile();
    ui->comboBox_track->addItems(TrackProvider::GetTracks());

    connect(RoverClientsProvider::GetRoverAutopilotClient()
            ,SIGNAL(StateUpdate(AutopilotState))
            ,this,SLOT(onAutopilotUpdate(AutopilotState)));

    auto sensorClient = RoverClientsProvider::GetRoverSensorClient();
    connect(sensorClient, SIGNAL(newData(RoverSensors)), this, SLOT(onSensorsUpdate(RoverSensors)));

    _run_on_autopilot = false;
}

RoverDriveForm::~RoverDriveForm()
{
    delete ui;
}

void RoverDriveForm::on_checkBox_toggled(bool checked)
{
    if(checked)
    {
        _run_on_autopilot = ui->comboBox_drive_type->currentText() != "Manual";
        UpdateSettings();
        if(_run_on_autopilot)
        {
            _timer->start(200);
        }else
        {
            _timer->start(_update_ms);
            _js->Start(_device_path);
        }
    }else
    {
        _timer->stop();
        _js->Stop();
    }
}

void RoverDriveForm::onUpdateDrive()
{
    if(!_run_on_autopilot)
    {
        updateManual();
    }
    else
    {
        updateAutopilot();
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

void RoverDriveForm::updateStats()
{
    OSMMarker * am = TrackProvider::ActivePoint();
    if(am == nullptr)
    {
        ui->lineEdit_tpos->setText("none");
        return;
    }

    ui->lineEdit_tpos->setText(QString("%1 %2").arg(am->possition.x()).arg(am->possition.y()));
    ui->spinBox_active_point->setValue(TrackProvider::ActivePointId());
}

void RoverDriveForm::updateManual()
{
    if(!RoverClientsProvider::GetRoverDriveClient()->IsConnected())
    {
        LoggerDialog::get()->Warning("Send drive command", "Drive client not connected.");
        ui->checkBox_drive->setChecked(false);
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

void RoverDriveForm::updateAutopilot()
{
    OSMMarker * p = TrackProvider::ActivePoint();
    if(p == nullptr)
    {
        RoverClientsProvider::GetRoverAutopilotClient()->SetAutopilot(false);
        return;
    }

    RoverClientsProvider::GetRoverAutopilotClient()->SetAutopilot(
                p->possition,
                TrackProvider::ActivePointId(),
                true);
}

void RoverDriveForm::on_pushButton_next_point_clicked()
{
    TrackProvider::NextPoint();
    updateStats();
}

void RoverDriveForm::on_pushButton_prev_point_clicked()
{
    TrackProvider::PrevPoint();
    updateStats();
}

void RoverDriveForm::on_pushButton_auto_clicked()
{
}

void RoverDriveForm::on_comboBox_drive_type_currentTextChanged(const QString &arg1)
{

}

void RoverDriveForm::on_comboBox_track_currentIndexChanged(const QString &arg1)
{
    TrackProvider::SelectRoute(arg1);
    updateStats();
}

void RoverDriveForm::on_spinBox_active_point_valueChanged(int arg1)
{
    if(TrackProvider::ActivePointId() == arg1)
        return;

    TrackProvider::SetActivePoint(arg1);
    ui->spinBox_active_point->setValue(TrackProvider::ActivePointId());
    updateStats();
}

void RoverDriveForm::on_comboBox_drive_type_currentIndexChanged(const QString &arg1)
{}

void RoverDriveForm::onAutopilotUpdate(AutopilotState state)
{
    ui->lineEdit_cd->setText(QString::number(state.angle));
    ui->lineEdit_dist->setText(QString::number(state.dist));
}

void RoverDriveForm::onSensorsUpdate(class RoverSensors readings)
{
    ui->lineEdit_cpos->setText(QString("%1 %2")
                                .arg(readings.gpsSensors.lognitude)
                                .arg(readings.gpsSensors.latitude));

    ui->lineEdit_cc->setText(QString::number(readings.gpsSensors.course));
}

void RoverDriveForm::on_checkBox_drive_toggled(bool checked)
{
    if(checked)
    {
        _run_on_autopilot = ui->comboBox_drive_type->currentText() != "Manual";
        UpdateSettings();
        if(_run_on_autopilot)
        {
            _timer->start(400);
        }else
        {

            _timer->start(_update_ms);
            _js->Start(_device_path);
        }
    }else
    {
        _timer->stop();
        _js->Stop();
    }
}
