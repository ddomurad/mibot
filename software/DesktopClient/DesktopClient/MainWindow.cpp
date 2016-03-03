#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentWidget(nullptr)
{
    ui->setupUi(this);
    ui->widget->setLayout( new QBoxLayout(QBoxLayout::LeftToRight,ui->widget));

    QAction * showSettings = ui->toolBar->addAction(QAction::tr("Settings"));
    QAction * showRouteEditor = ui->toolBar->addAction(QAction::tr("RouteEditor"));
    QAction * showCtrlPanel = ui->toolBar->addAction(QAction::tr("CtrlPanel"));

    connect(showSettings, SIGNAL(triggered()), this, SLOT(onShowSettings()));
    connect(showRouteEditor, SIGNAL(triggered()), this, SLOT(onShowEditor()));
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

void MainWindow::onShowCtrlPanel()
{
    closeCurrentWidget();

    currentWidget = new CtrlPanelForm(ui->widget);
    ui->widget->layout()->addWidget(currentWidget);
}

void MainWindow::closeCurrentWidget()
{

    if(currentWidget != nullptr)
    {
        ui->widget->layout()->removeWidget(currentWidget);
        currentWidget->deleteLater();
        currentWidget = nullptr;
    }
}
