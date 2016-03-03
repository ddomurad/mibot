#ifndef ROVERCONNECTIONFORM_H
#define ROVERCONNECTIONFORM_H

#include <QWidget>

namespace Ui {
class RoverConnectionForm;
}

class RoverConnectionForm : public QWidget
{
    Q_OBJECT

public:
    explicit RoverConnectionForm(QWidget *parent = 0);
    ~RoverConnectionForm();

    static int GetRefCnt();
private slots:
    void on_pushButton_clicked();

private:
    Ui::RoverConnectionForm *ui;

    static int _ref_count;
};

#endif // ROVERCONNECTIONFORM_H
