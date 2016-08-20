#ifndef PICTUREVIEWFORM_H
#define PICTUREVIEWFORM_H

#include <QWidget>
#include "ReferenceCounter.h"


namespace Ui {
class PictureViewForm;
}

class PictureViewForm : public QWidget, public ReferenceCounter<PictureViewForm>
{
    Q_OBJECT

public:
    explicit PictureViewForm(QWidget *parent = 0);
    ~PictureViewForm();
public slots:
    void onImage(QImage * image);
private slots:
    void on_get_clicked();
    void on_auto_get_toggled(bool checked);
    void on_timer();

private:
    Ui::PictureViewForm *ui;
    void getPicture();

    QTimer *timer;
    bool auto_get;
};

#endif // PICTUREVIEWFORM_H
