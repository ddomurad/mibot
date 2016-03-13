#include "./Forms/LoggerDialog.h"
#include "./Settings/AppSettings.h"
#include "./Forms/InfoServiceForm.h"
#include "ui_InfoServiceForm.h"

InfoServiceForm::InfoServiceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InfoServiceForm)
{
    ui->setupUi(this);
    _infoClient = new InfoServiceClient(this);

    connect(_infoClient, SIGNAL(onRefreshDone()), this, SLOT(onRefreshDone()));
}

InfoServiceForm::~InfoServiceForm()
{
    delete ui;
}

void InfoServiceForm::on_pushButton_refresh_clicked()
{
    ui->tableWidget->clear();
    _infoClient->Refresh();
}

void InfoServiceForm::onRefreshDone()
{
    QList<ServiceInfo> infos = _infoClient->GetServiceInfos();
    ui->tableWidget->setRowCount(infos.count());
    ui->tableWidget->setColumnCount(5);

    QStringList headers;
    headers << "Name" << "Type" << "Port" << "SSL" << "Access Level";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    for(int i=0;i<infos.count(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(infos[i].name));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(infos[i].type));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(infos[i].port)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(infos[i].ssl)));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(infos[i].accessLevel)));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem());
    }
}
