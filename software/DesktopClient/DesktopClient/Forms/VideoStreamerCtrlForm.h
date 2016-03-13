#ifndef VIDEOSTREAMERCTRLFORM_H
#define VIDEOSTREAMERCTRLFORM_H

#include <QWidget>
#include "ReferenceCounter.h"
#include "./Clients/RoverClientsProvider.h"

namespace Ui {
class VideoStreamerCtrlForm;
}

class VideoStreamerCtrlForm : public QWidget, public ReferenceCounter<VideoStreamerCtrlForm>
{
    Q_OBJECT

public:
    explicit VideoStreamerCtrlForm(QWidget *parent = 0);
    ~VideoStreamerCtrlForm();

private slots:
    void on_pushButton_run_clicked();

    void on_pushButton_stop_clicked();

private:
    Ui::VideoStreamerCtrlForm *ui;

    VideoServiceClient * _videoClient;
};

#endif // VIDEOSTREAMERCTRLFORM_H
