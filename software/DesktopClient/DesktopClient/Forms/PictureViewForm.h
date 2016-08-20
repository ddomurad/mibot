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

private:
    Ui::PictureViewForm *ui;
};

#endif // PICTUREVIEWFORM_H
