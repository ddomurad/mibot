#include "Forms/PictureViewForm.h"
#include "ui_PictureViewForm.h"
#include "Clients/RoverClientsProvider.h"

PictureViewForm::PictureViewForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureViewForm)
{
    ui->setupUi(this);

    PictureClient * client = RoverClientsProvider::GetPictureClient();
    connect(client, SIGNAL(onImage(QImage*)), this, SLOT(onImage(QImage*)));
}

PictureViewForm::~PictureViewForm()
{
    delete ui;
}

void PictureViewForm::onImage(QImage *image)
{
    QImage newImage = image->copy();
    ui->picture->setPixmap(QPixmap::fromImage(newImage));
}

void PictureViewForm::on_get_clicked()
{
    PictureClient * client = RoverClientsProvider::GetPictureClient();
    client->RequestPicture(320,240,10);
}
