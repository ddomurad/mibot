#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <JsInput.h>

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

    void on_btn_Check_js_clicked();

    void on_cb_js_run_toggled(bool checked);

    void onSendTimer();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_btn_send_plain_text_clicked();

    void on_cb_repeat_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    QTcpSocket * socket;
    void Log(QString type, QString msg);
    QTimer *send_timer;
    JsInput * js_input_thread;

    void stopSending();
    int send_type;

    void sendJsState();
    void sendText();
};

#endif // MAINWINDOW_H
