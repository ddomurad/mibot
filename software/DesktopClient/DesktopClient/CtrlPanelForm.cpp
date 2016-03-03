#include "CtrlPanelForm.h"
#include "ui_CtrlPanelForm.h"

CtrlPanelForm::CtrlPanelForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CtrlPanelForm)
{
    ui->setupUi(this);
}

CtrlPanelForm::~CtrlPanelForm()
{
    delete ui;
}
