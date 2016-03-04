#ifndef SOUNDSIGNALLERFORM_H
#define SOUNDSIGNALLERFORM_H

#include "ReferenceCounter.h"
#include <QWidget>


namespace Ui {
class SoundSignallerForm;
}

class SoundSignallerForm : public QWidget, public ReferenceCounter<SoundSignallerForm>
{
    Q_OBJECT

public:
    explicit SoundSignallerForm(QWidget *parent = 0);
    ~SoundSignallerForm();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_pressed();
    void on_pushButton_released();

private:
    Ui::SoundSignallerForm *ui;

    void SetPiezoState(bool state);
};

#endif // SOUNDSIGNALLERFORM_H
