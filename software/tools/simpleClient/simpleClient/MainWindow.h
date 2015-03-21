#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

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
    void on_btn_connect_clicked();

    void onSocketError(QAbstractSocket::SocketError);
    void onPeerVerifyError(QSslError);
    void onSslErrors(QList<QSslError>);
    void onConnected();
    void onReadyRead();
    void onBytesWritten(qint64);

    void on_btn_send_clicked();

    void on_checkBox_toggled(bool checked);

    void onTimer();
private:
    Ui::MainWindow *ui;
    QTcpSocket * socket;
    QTimer *timer;
    void Log(QString type, QString msg);
};

#endif // MAINWINDOW_H
