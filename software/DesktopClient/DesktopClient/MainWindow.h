#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <SettingsForm.h>
#include <RouteEditorForm.h>

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

private:
    Ui::MainWindow *ui;

    void closeCurrentWidget();
    QWidget *currentWidget;
};

#endif // MAINWINDOW_H
