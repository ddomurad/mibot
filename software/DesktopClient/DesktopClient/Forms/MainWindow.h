#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "./Forms/SettingsForm.h"
#include "./Forms/RouteEditorForm.h"
#include "./Forms/CtrlPanelForm.h"
#include "./Forms/InfoServiceForm.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onShowSettings();
    void onShowEditor();
    void onShowInfoServicePanel();
    void onShowCtrlPanel();

private:
    Ui::MainWindow *ui;

    void closeCurrentWidget();
    QWidget *currentWidget;
    QWidget *ctrlWidget;
};

#endif // MAINWINDOW_H
