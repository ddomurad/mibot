#include <QMdiSubWindow>
#include "LoggerDialog.h"
#include "CtrlPanelForm.h"
#include "ui_CtrlPanelForm.h"

CtrlPanelForm::CtrlPanelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlPanelForm),
    _roverConnectionForm(nullptr),
    _sensorReaderForm(nullptr),
    _soundSignallerForm(nullptr),
    _videoStreamerCtrlForm(nullptr),
    _videoReceiverForm(nullptr),
    _gpsPreview(nullptr)
{
    ui->setupUi(this);
}

CtrlPanelForm::~CtrlPanelForm()
{
    delete ui;
}

void CtrlPanelForm::on_toolButton_sensors_clicked()
{
    if(SensorsReaderForm::GetRefCount() == 0)
    {
        _sensorReaderForm = new SensorsReaderForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_sensorReaderForm);
        _sensorReaderForm->show();
    }
}

void CtrlPanelForm::on_toolButton_connection_clicked()
{
    if(RoverConnectionForm::GetRefCount() == 0)
    {
        _roverConnectionForm = new RoverConnectionForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_roverConnectionForm);
        _roverConnectionForm->show();
    }
}

void CtrlPanelForm::on_toolButton_sound_clicked()
{
    if(SoundSignallerForm::GetRefCount() == 0)
    {
        _soundSignallerForm = new SoundSignallerForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_soundSignallerForm);
        _soundSignallerForm->show();
    }
}


void CtrlPanelForm::on_toolButton_drive_clicked()
{
    if(RoverDriveForm::GetRefCount() == 0)
    {
        _roverDriveForm = new RoverDriveForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_roverDriveForm);
        _roverDriveForm->show();
    }
}

void CtrlPanelForm::on_toolButton_plots_clicked()
{
    if(PlotFactoryForm::GetRefCount() == 0)
    {
        _plotFormFactory = new PlotFactoryForm(ui->mdiArea, ui->mdiArea);
        ui->mdiArea->addSubWindow(_plotFormFactory);
        _plotFormFactory->show();
    }
}

void CtrlPanelForm::on_toolButton_video_controll_clicked()
{
    if(VideoStreamerCtrlForm::GetRefCount() == 0)
    {
        _videoStreamerCtrlForm = new VideoStreamerCtrlForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_videoStreamerCtrlForm);
        _videoStreamerCtrlForm->show();
    }
}

void CtrlPanelForm::on_toolButton_clicked()
{
    if(VideoReceiverForm::GetRefCount() == 0)
    {
        _videoReceiverForm = new VideoReceiverForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_videoReceiverForm);
        _videoReceiverForm->show();
    }
}



void CtrlPanelForm::on_toolButton_2_clicked()
{
    if(GpsPreviewForm::GetRefCount() == 0)
    {
        _gpsPreview = new GpsPreviewForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_gpsPreview);
        _gpsPreview ->show();
    }
}
