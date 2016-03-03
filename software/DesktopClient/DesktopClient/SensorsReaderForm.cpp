#include "SensorsReaderForm.h"
#include "ui_SensorsReaderForm.h"

SensorsReaderForm::SensorsReaderForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsReaderForm)
{
    ui->setupUi(this);
}

SensorsReaderForm::~SensorsReaderForm()
{
    delete ui;
}
