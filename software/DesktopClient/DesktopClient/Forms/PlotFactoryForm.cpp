#include "PlotFactoryForm.h"
#include "ui_PlotFactoryForm.h"
#include "DataSourceProvider.h"
#include "Plots/LinePlotForm.h"
#include "Plots/LinePlot2Form.h"

PlotFactoryForm::PlotFactoryForm(QMdiArea * mdiArea, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotFactoryForm)
{
    ui->setupUi(this);

    QList<QString> sourcesNames = DataSourceProvider::get()->DataSources();

    for(QString name : sourcesNames)
    {
        ui->listWidget->addItem(name);
        ui->listWidget_x_data->addItem(name);
        ui->listWidget_y_data->addItem(name);
    }

    this->mdiArea = mdiArea;
}

PlotFactoryForm::~PlotFactoryForm()
{
    delete ui;
}

void PlotFactoryForm::on_pushButton_clicked()
{
    if(ui->listWidget->currentItem() == nullptr)
        return;

    LinePlotForm * linePlot = new LinePlotForm(mdiArea);
    linePlot->SetDataSource(DataSourceProvider::get()->GetSource(ui->listWidget->currentItem()->text()));
    mdiArea->addSubWindow(linePlot);
    linePlot->show();
}

void PlotFactoryForm::on_pushButton_2_clicked()
{
    if(ui->listWidget_x_data->currentItem() == nullptr)
        return;

    if(ui->listWidget_y_data->currentItem() == nullptr)
        return;

    LinePlot2Form * lin2Plot = new LinePlot2Form(mdiArea);
    lin2Plot->SetDataSource(
                DataSourceProvider::get()->GetSource(ui->listWidget_x_data->currentItem()->text()),
                DataSourceProvider::get()->GetSource(ui->listWidget_y_data->currentItem()->text()));
    lin2Plot->UpdateVisibleAreaFromDataSource();
    mdiArea->addSubWindow(lin2Plot );
    lin2Plot ->show();
}
