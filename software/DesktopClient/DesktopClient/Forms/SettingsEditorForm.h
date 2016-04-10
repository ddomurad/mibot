#ifndef SETTINGSEDITORFORM_H
#define SETTINGSEDITORFORM_H

#include <QWidget>
#include <QSslSocket>
#include <QSslCertificate>
#include "Clients/mibJSONProtocol.h"
#include <QTreeWidgetItem>

namespace Ui {
class SettingsEditorForm;
}

class SettingsEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsEditorForm(QWidget *parent = 0);
    ~SettingsEditorForm();

private slots:
    void onSocketError(QAbstractSocket::SocketError);
    void onPeerVerifyError(QSslError);
    void onSslErrors(QList<QSslError>);
    void onConnected();
    void onReadyRead();

    void on_pushButton_connect_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_load_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_remove_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::SettingsEditorForm *ui;

    QSslSocket *_socket;

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

#endif // SETTINGSEDITORFORM_H
