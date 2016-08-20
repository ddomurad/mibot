#ifndef CTRLPANELFORM_H
#define CTRLPANELFORM_H

#include <QWidget>
#include "RoverConnectionForm.h"
#include "SensorsReaderForm.h"
#include "SoundSignallerForm.h"
#include "RoverDriveForm.h"
#include "PlotFactoryForm.h"
#include "VideoStreamerCtrlForm.h"
#include "VideoReceiverForm.h"
#include "GpsPreviewForm.h"
#include "RecordingForm.h"
#include "PictureViewForm.h"

namespace Ui {
class CtrlPanelForm;
}

class CtrlPanelForm : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlPanelForm(QWidget *parent = 0);
    ~CtrlPanelForm();

private slots:
    void on_toolButton_sensors_clicked();

    void on_toolButton_connection_clicked();

    void on_toolButton_sound_clicked();

    void on_toolButton_drive_clicked();

    void on_toolButton_plots_clicked();

    void on_toolButton_video_controll_clicked();

    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_toolButton_recorder_clicked();

    void on_toolButton_pict_clicked();

private:
    Ui::CtrlPanelForm *ui;

    RoverConnectionForm * _roverConnectionForm;
    SensorsReaderForm * _sensorReaderForm;
    SoundSignallerForm * _soundSignallerForm;
    RoverDriveForm * _roverDriveForm;
    PlotFactoryForm * _plotFormFactory;
    VideoStreamerCtrlForm * _videoStreamerCtrlForm;
    VideoReceiverForm *_videoReceiverForm;
    GpsPreviewForm *_gpsPreview;
    RecordingForm * _recorderForm;
    PictureViewForm * _pictureViewForm;
};

#endif // CTRLPANELFORM_H
