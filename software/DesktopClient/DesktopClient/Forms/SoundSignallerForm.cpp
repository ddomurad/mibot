#include "SoundSignallerForm.h"
#include "ui_SoundSignallerForm.h"
#include "Clients/RoverClientsProvider.h"
#include "LoggerDialog.h"

SoundSignallerForm::SoundSignallerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SoundSignallerForm)
{
    ui->setupUi(this);
}

SoundSignallerForm::~SoundSignallerForm()
{
    delete ui;
}

void SoundSignallerForm::on_pushButton_2_clicked()
{
    SetPiezoState(true);
}

void SoundSignallerForm::on_pushButton_3_clicked()
{
    SetPiezoState(false);
}

void SoundSignallerForm::on_pushButton_pressed()
{
    SetPiezoState(true);
}

void SoundSignallerForm::on_pushButton_released()
{
    SetPiezoState(false);
}

void SoundSignallerForm::SetPiezoState(bool state)
{
    RoverSensorClient * client =  RoverClientsProvider::GetRoverSensorClient();
    if(!client->IsConnected())
    {
        LoggerDialog::get()->Warning("Change piezo state", "Sensor client is disconnected.");
        return;
    }

    if(state)
        client->SendData("{\"piezo\":1}");
    else
        client->SendData("{\"piezo\":0}");
}
