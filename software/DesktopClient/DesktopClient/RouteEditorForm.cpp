#include "RouteEditorForm.h"
#include "ui_RouteEditorForm.h"

RouteEditorForm::RouteEditorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RouteEditorForm)
{
    ui->setupUi(this);
}

RouteEditorForm::~RouteEditorForm()
{
    delete ui;
}
