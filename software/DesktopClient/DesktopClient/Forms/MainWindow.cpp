#include "./Forms/MainWindow.h"
#include "ui_MainWindow.h"
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentWidget(nullptr),
    ctrlWidget(nullptr)
{
    ui->setupUi(this);
    ui->widget->setLayout( new QBoxLayout(QBoxLayout::LeftToRight,ui->widget));

    QAction * showSettings = ui->toolBar->addAction(QAction::tr("Settings"));
    QAction * showRouteEditor = ui->toolBar->addAction(QAction::tr("RouteEditor"));
    QAction * showInfoService = ui->toolBar->addAction(QAction::tr("InfoService"));
    QAction * showSettingsPanel = ui->toolBar->addAction(QAction::tr("SettingsManager"));
    QAction * showCtrlPanel = ui->toolBar->addAction(QAction::tr("CtrlPanel"));

    connect(showSettings, SIGNAL(triggered()), this, SLOT(onShowSettings()));
    connect(showRouteEditor, SIGNAL(triggered()), this, SLOT(onShowEditor()));
    connect(showInfoService, SIGNAL(triggered()), this, SLOT(onShowInfoServicePanel()));
    connect(showSettingsPanel, SIGNAL(triggered()), this, SLOT(onShowSettingsPanel()));
    connect(showCtrlPanel, SIGNAL(triggered()), this, SLOT(onShowCtrlPanel()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onShowSettings()
{
    closeCurrentWidget();

    currentWidget = new SettingsForm(ui->widget);
    ui->widget->layout()->addWidget(currentWidget);
}

void MainWindow::onShowEditor()
{
    closeCurrentWidget();

    currentWidget = new RouteEditorForm(ui->widget);
    ui->widget->layout()->addWidget(currentWidget);
}

void MainWindow::onShowInfoServicePanel()
{
    closeCurrentWidget();

    currentWidget = new InfoServiceForm(ui->widget);
    ui->widget->layout()->addWidget(currentWidget);
}

void MainWindow::onShowSettingsPanel()
{
    closeCurrentWidget();

    currentWidget = new SettingsEditorForm(ui->widget);
    ui->widget->layout()->addWidget(currentWidget);
}

void MainWindow::onShowCtrlPanel()
{
    closeCurrentWidget();

    if(ctrlWidget == nullptr)
    {
        ctrlWidget = new CtrlPanelForm(ui->widget);
        ui->widget->layout()->addWidget(ctrlWidget);
    }
    ctrlWidget->setVisible(true);
}

void MainWindow::closeCurrentWidget()
{
        if(ctrlWidget != nullptr)
        ctrlWidget->setVisible(false);
    if(currentWidget != nullptr)
    {
        ui->widget->layout()->removeWidget(currentWidget);
        currentWidget->deleteLater();
        currentWidget = nullptr;
    }
}
