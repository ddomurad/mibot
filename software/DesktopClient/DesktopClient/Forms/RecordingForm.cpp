#include "Other/Replayer.h"
#include <QDateTime>
#include "Forms/RecordingForm.h"
#include "ui_RecordingForm.h"
#include "Other/Recorder.h"
#include <QTimer>

RecordingForm::RecordingForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RecordingForm)
{
    ui->setupUi(this);
    ui->lineEdit_recname->setText( "rec_" + QDateTime::currentDateTime().toString("dd_MM_yyyy__HH_mm"));

    QPalette palete = ui->label_recorder_state->palette();
    ui->label_recorder_state->setText("[ REC ]");
    ui->label_recorder_state->setVisible(false);
    palete .setColor(ui->label_recorder_state->foregroundRole(), Qt::red);
    ui->label_recorder_state->setPalette(palete);

    _rec_timer = new QTimer(this);
    connect(_rec_timer, SIGNAL(timeout()), this, SLOT(onRecTimer()));

    ui->pushButton_start_stop_record->setText("Start recording");

    ui->groupBox_ctrl->setEnabled(false);
    ui->groupBox_list->setEnabled(true);
    ui->groupBox_start_stop->setEnabled(false);

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()),this,SLOT(onTimer()));
}

RecordingForm::~RecordingForm()
{
    delete ui;
}

void RecordingForm::on_pushButton_start_stop_record_clicked()
{
    if(Recorder::get()->IsRecording())
        stopRectording();
    else
        startRecording();
}

void RecordingForm::onRecTimer()
{
    ui->label_recorder_state->setVisible( !ui->label_recorder_state->isVisible() );
}

void RecordingForm::startRecording()
{
    if(!Recorder::get()->StartRecording(ui->lineEdit_recname->text()))
        return;

    _rec_timer->start(1000);
    ui->pushButton_start_stop_record->setText("Stop recording");
}

void RecordingForm::stopRectording()
{
    _rec_timer->stop();
    ui->label_recorder_state->setVisible(false);
    Recorder::get()->StopRecording();

    ui->pushButton_start_stop_record->setText("Start recording");
}

void RecordingForm::on_listWidget_rectords_clicked(const QModelIndex &index)
{
    ui->groupBox_start_stop->setEnabled(ui->listWidget_rectords->currentRow() != -1);
}

void RecordingForm::on_pushButton_refresh_clicked()
{
    QString recordsDir = AppSettings::GetKey(AppSettings::Recording_Store_Dir).toString();
    QDir dir;
    dir.setCurrent(recordsDir);
    ui->listWidget_rectords->clear();
    ui->listWidget_rectords->addItems(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot));
}

void RecordingForm::on_pushButton_start_replay_clicked()
{
    if(Replayer::get()->IsInitialized())
    {
        ui->pushButton_start_replay->setEnabled(false);
        return;
    }

    QString recName = ui->listWidget_rectords->currentItem()->text();
    if(!Replayer::get()->InitReplayer(recName))
    {
        Replayer::get()->Deinitialize();
        return;
    }

    ui->pushButton_start_replay->setEnabled(false);
    ui->pushButton_stop_replay->setEnabled(true);
    ui->groupBox_ctrl->setEnabled(true);
}

void RecordingForm::on_pushButton_stop_replay_clicked()
{
    ui->pushButton_start_replay->setEnabled(true);
    ui->pushButton_stop_replay->setEnabled(false);

    Replayer::get()->StopReplay();
    Replayer::get()->Deinitialize();
    ui->groupBox_ctrl->setEnabled(false);
}

void RecordingForm::onTimer()
{
    Replayer::get()->UpdateReplay();
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    qint64 nextTime = Replayer::get()->NextDataChunkTime();
    qint64 timeDiff = nextTime - currentTime;

    if(timeDiff > 0)
        _timer->setInterval(timeDiff);
    else
        _timer->setInterval(0);
}

void RecordingForm::on_pushButton_resume_clicked()
{
    if(Replayer::get()->IsReplayOn())
        return;

    Replayer::get()->StartReplay();
    _timer->start(0);
}

void RecordingForm::on_pushButton_pause_clicked()
{
    Replayer::get()->StopReplay();
    _timer->stop();
}
