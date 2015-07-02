#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSslSocket>
#include <QSslCertificate>
#include <mibJSONProtocol.h>
#include <QTreeWidgetItem>

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
    void onSocketError(QAbstractSocket::SocketError);
    void onPeerVerifyError(QSslError);
    void onSslErrors(QList<QSslError>);
    void onConnected();
    void onReadyRead();

    void on_pushButton_connect_to_server_clicked();

    void on_pushButton_refreshdir_list_clicked();

    void on_pushButton_explode_clicked();

    void on_pushButton_colapse_clicked();

    void on_treeWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_load_json_clicked();

    void on_pushButton_save_json_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_remove_clicked();

private:
    Ui::MainWindow *ui;

    QSslSocket *_socket;

    void updateConnectButtonText();
    void Log(QString typ, QString message);

    void connectToHost(QString addr, int port);
    void disonnectFromHost();

    void fixProtocol();
    mibot::JSONProtocol _protocol;

    void processProtocol();
    void processCommand(QJsonObject &obj);
    void processListResp(QJsonObject &obj);
    void processGetResp(QJsonObject &obj);

    QString composeListCommand(QString dir);
    QString composeGetCommand(QString res);
    QString composeSetCommand(QString res, QString data);
    QString composeRemoveCommand(QString res);

    void trySendData(QByteArray arr);
    void trySendData(QString arr);

    void updateTreeList(QString path, QStringList newDirs, QStringList newElements);
    void requestNewLists(QString path, QStringList newDirs);
    void reloadTreeList();
};

#endif // MAINWINDOW_H
