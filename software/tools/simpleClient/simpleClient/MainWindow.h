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

private:
    Ui::MainWindow *ui;
    QSslSocket * socket;

    void Log(QString type, QString msg);
};

#endif // MAINWINDOW_H
