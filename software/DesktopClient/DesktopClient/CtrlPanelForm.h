#ifndef CTRLPANELFORM_H
#define CTRLPANELFORM_H

#include <QWidget>
#include "RoverConnectionForm.h"

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

private:
    Ui::CtrlPanelForm *ui;

    RoverConnectionForm * _roverConnectionForm;
};

#endif // CTRLPANELFORM_H
