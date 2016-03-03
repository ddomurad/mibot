
#include "LoggerDialog.h"
#include "CtrlPanelForm.h"
#include "ui_CtrlPanelForm.h"

CtrlPanelForm::CtrlPanelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlPanelForm),
    _roverConnectionForm(nullptr)
{
    ui->setupUi(this);
}

CtrlPanelForm::~CtrlPanelForm()
{
    delete ui;
}

void CtrlPanelForm::on_toolButton_sensors_clicked()
{

}

void CtrlPanelForm::on_toolButton_connection_clicked()
{
    if(RoverConnectionForm::GetRefCnt() == 0)
    {
        _roverConnectionForm = new RoverConnectionForm(ui->mdiArea);
        ui->mdiArea->addSubWindow(_roverConnectionForm);
        _roverConnectionForm->show();
    }
}
