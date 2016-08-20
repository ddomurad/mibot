#include "Forms/PictureViewForm.h"
#include "ui_PictureViewForm.h"
#include "Clients/RoverClientsProvider.h"

PictureViewForm::PictureViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureViewForm)
{
    ui->setupUi(this);

    auto_get = false;
    PictureClient * client = RoverClientsProvider::GetPictureClient();
    connect(client, SIGNAL(onImage(QImage*)), this, SLOT(onImage(QImage*)));

    ui->spinBox_w->setValue(320);
    ui->spinBox_h->setValue(240);
    ui->spinBox_q->setValue(10);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timer()));
}

PictureViewForm::~PictureViewForm()
{
    delete ui;
}

void PictureViewForm::onImage(QImage *image)
{
    QImage newImage = image->copy();
    ui->picture->setPixmap(QPixmap::fromImage(newImage));

    if(auto_get)
        timer->start(ui->sec->value() * 1000);
}

void PictureViewForm::getPicture()
{
    PictureClient * client = RoverClientsProvider::GetPictureClient();
    client->RequestPicture(ui->spinBox_w->value(),ui->spinBox_h->value(), ui->spinBox_q->value());
}

void PictureViewForm::on_get_clicked()
{
    getPicture();
}

void PictureViewForm::on_auto_get_toggled(bool checked)
{
    if(checked && !auto_get)
        timer->start(ui->sec->value() * 1000);

    auto_get = checked;
}

void PictureViewForm::on_timer()
{
    timer->stop();
    getPicture();
}
