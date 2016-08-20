#ifndef PICTUREVIEWFORM_H
#define PICTUREVIEWFORM_H

#include <QWidget>

namespace Ui {
class PictureViewForm;
}

class PictureViewForm : public QWidget
{
    Q_OBJECT

public:
    explicit PictureViewForm(QWidget *parent = 0);
    ~PictureViewForm();

public slots:
    void onImage(QImage * image);
private slots:
    void on_get_clicked();

private:
    Ui::PictureViewForm *ui;
};

#endif // PICTUREVIEWFORM_H
