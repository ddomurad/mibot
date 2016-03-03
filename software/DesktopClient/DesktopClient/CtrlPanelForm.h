#ifndef CTRLPANELFORM_H
#define CTRLPANELFORM_H

#include <QWidget>

namespace Ui {
class CtrlPanelForm;
}

class CtrlPanelForm : public QWidget
{
    Q_OBJECT

public:
    explicit CtrlPanelForm(QWidget *parent = 0);
    ~CtrlPanelForm();

private:
    Ui::CtrlPanelForm *ui;
};

#endif // CTRLPANELFORM_H
