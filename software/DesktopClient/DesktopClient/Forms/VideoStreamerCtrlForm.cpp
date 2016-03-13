#include "LoggerDialog.h"
#include "VideoStreamerCtrlForm.h"
#include "ui_VideoStreamerCtrlForm.h"

VideoStreamerCtrlForm::VideoStreamerCtrlForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoStreamerCtrlForm)
{
    ui->setupUi(this);

    _videoClient = RoverClientsProvider::GetVideoServiceClient();
}

VideoStreamerCtrlForm::~VideoStreamerCtrlForm()
{
    delete ui;
}

void VideoStreamerCtrlForm::on_pushButton_run_clicked()
{
    if(!_videoClient->IsConnected())
    {
        LoggerDialog::get()->Warning("Start video streamer", "Video client is not connected.");
        return;
    }

    QString resolutionText = ui->comboBox_resolution->currentText();
    QStringList resolutionStringList = resolutionText.split('x');

    int videoWight = resolutionStringList[0].toInt();
    int videoHeight = resolutionStringList[1].toInt();

    _videoClient->StartStreamer(videoWight, videoHeight,
                                ui->lineEdit_device->text(),
                                ui->lineEdit_server_addr->text(),
                                ui->spinBox_server_port->value(),
                                "h264",3);
}

void VideoStreamerCtrlForm::on_pushButton_stop_clicked()
{
    if(!_videoClient->IsConnected())
    {
        LoggerDialog::get()->Warning("Stop video streamer", "Video client is not connected.");
        return;
    }

    _videoClient->StopStreamer();
}
