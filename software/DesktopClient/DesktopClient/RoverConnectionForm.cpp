#include "RoverConnectionForm.h"
#include "ui_RoverConnectionForm.h"

RoverConnectionForm::RoverConnectionForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoverConnectionForm)
{
    ui->setupUi(this);
    _ref_count++;
}

RoverConnectionForm::~RoverConnectionForm()
{
    _ref_count--;
    delete ui;
}

int RoverConnectionForm::GetRefCnt()
{
    return _ref_count;
}

void RoverConnectionForm::on_pushButton_clicked()
{

}

int RoverConnectionForm::_ref_count = 0;
