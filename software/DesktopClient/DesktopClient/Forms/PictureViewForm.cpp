#include "Forms/PictureViewForm.h"
#include "ui_PictureViewForm.h"

PictureViewForm::PictureViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureViewForm)
{
    ui->setupUi(this);
}

PictureViewForm::~PictureViewForm()
{
    delete ui;
}
