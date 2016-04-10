#ifndef RECORDINGFORM_H
#define RECORDINGFORM_H

#include <QWidget>
#include "ReferenceCounter.h"

namespace Ui {
class RecordingForm;
}

class RecordingForm : public QWidget, public ReferenceCounter<RecordingForm>
{
    Q_OBJECT

public:
    explicit RecordingForm(QWidget *parent = 0);
    ~RecordingForm();

private slots:
    void on_pushButton_start_stop_record_clicked();
    void onRecTimer();
    void on_listWidget_rectords_clicked(const QModelIndex &index);

    void on_pushButton_refresh_clicked();

    void on_pushButton_start_replay_clicked();

    void on_pushButton_stop_replay_clicked();

    void onTimer();
    void on_pushButton_resume_clicked();

    void on_pushButton_pause_clicked();

private:
    Ui::RecordingForm *ui;
    QTimer * _rec_timer;

    void startRecording();
    void stopRectording();
    QTimer *_timer;
};

#endif // RECORDINGFORM_H
