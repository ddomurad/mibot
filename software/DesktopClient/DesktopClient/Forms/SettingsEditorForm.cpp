#include "LoggerDialog.h"
#include "SettingsEditorForm.h"
#include "ui_SettingsEditorForm.h"

#define TWT_DIR 0x01
#define TWT_RES 0x02

SettingsEditorForm::SettingsEditorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsEditorForm),
    _socket(nullptr)
{
    ui->setupUi(this);
}

SettingsEditorForm::~SettingsEditorForm()
{
    delete ui;
}

void SettingsEditorForm::onSocketError(QAbstractSocket::SocketError)
{
    LoggerDialog::get()->Error("SettingsEditorForm::onSocketError",QString("Socket error (%1)").arg( ((QSslSocket*)sender())->errorString() ));
    _socket->close();
    ui->pushButton_connect->setText("Connect");
}

void SettingsEditorForm::onPeerVerifyError(QSslError err)
{
    LoggerDialog::get()->Error("SettingsEditorForm::onPeerVerifyError",QString("PeerVerifyError (%1)").arg(err.errorString()));
    _socket->close();
    ui->pushButton_connect->setText("Connect");
}

void SettingsEditorForm::onSslErrors(QList<QSslError> errs)
{
    for(QSslError err : errs  )
        LoggerDialog::get()->Error("SettingsEditorForm::onSslErrors",QString("SslErrors (%1)").arg(err.errorString()));
    _socket->close();
    ui->pushButton_connect->setText("Connect");
}

void SettingsEditorForm::onConnected()
{
    LoggerDialog::get()->Info("SettingsEditorForm::onConnected","Connected");
    ui->pushButton_connect->setText("Disconnect");
}

void SettingsEditorForm::onReadyRead()
{
    QByteArray data = _socket->readAll();
    _protocol.PushData(data);
    fixProtocol();
    processProtocol();
}

void SettingsEditorForm::on_pushButton_connect_clicked()
{
    if(ui->pushButton_connect->text() == "Connect")
        connectToHost(ui->lineEdit_host->text(), ui->spinBox_port->value());
    else
        disonnectFromHost();
}

void SettingsEditorForm::on_pushButton_clicked()
{
    reloadTreeList();
}

void SettingsEditorForm::on_pushButton_2_clicked()
{
    ui->treeWidget->expandAll();
}

void SettingsEditorForm::on_pushButton_3_clicked()
{
    ui->treeWidget->collapseAll();
}

void SettingsEditorForm::on_pushButton_load_clicked()
{
    trySendData(composeGetCommand(ui->lineEdit_res_path->text()));
}

void SettingsEditorForm::on_pushButton_save_clicked()
{
    trySendData(composeSetCommand(ui->lineEdit_res_path->text(), ui->textEdit->toPlainText()));
}

void SettingsEditorForm::on_pushButton_remove_clicked()
{
    trySendData( composeRemoveCommand( ui->lineEdit_res_path->text() ));
}

void SettingsEditorForm::connectToHost(QString addr, int port)
{
    if(_socket != nullptr)
    {
        _socket->close();
        _socket->deleteLater();
    }

    _socket = new QSslSocket(this);
    connect( _socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(onSocketError(QAbstractSocket::SocketError)));

    connect( _socket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(onPeerVerifyError(QSslError)) );
    connect( _socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));

    connect( _socket, SIGNAL(connected()),this,SLOT(onConnected()));
    connect( _socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    _socket->setLocalCertificate( "./certs/manager.crt"  );
    _socket->setPrivateKey( "./certs/manager.key");

    _socket->setProtocol(QSsl::SecureProtocols);

    _socket->setCaCertificates(QSslCertificate::fromPath("./certs/trusted.pem"));

    _socket->connectToHostEncrypted( addr, port, "SettingsServer");

    if(!_socket->waitForEncrypted(1000))
    {
        LoggerDialog::get()->Error("SettingsEditorForm::connectToHost","Wait for encrypted failed!");
        _socket->close();
        ui->pushButton_connect->setText("Connect");
    }
}

void SettingsEditorForm::disonnectFromHost()
{
    if(_socket == nullptr) return;
    _socket->close();
    ui->pushButton_connect->setText("Connect");
}

void SettingsEditorForm::fixProtocol()
{
    if(_protocol.IsDataCorupted())
    {
        LoggerDialog::get()->Warning("SettingsEditorForm::fixProtocol","Cleaning protocol corupted data");
        _protocol.RemoveCoruptedData();
        if(_protocol.IsDataCorupted())
        {
            LoggerDialog::get()->Warning("SettingsEditorForm::fixProtocol","Can't clean corupted data. Removing everythig.");
            _protocol.RemoveAllData();
        }
    }
}

void SettingsEditorForm::processProtocol()
{
    while(_protocol.ContainsValidJSONObject())
    {
        QJsonObject command = _protocol.GetPendingObject();
        processCommand( command );
        fixProtocol();
    }
}

void SettingsEditorForm::processCommand(QJsonObject &obj)
{
    QString command = obj["Command"].toString();

    if(command == "List-Resp")
    {
        processListResp(obj);
    }
    else if(command == "Get-Resp")
    {
        processGetResp(obj);
    }
    else if(command == "Remove-Resp" || command == "Set-Resp")
    {
        reloadTreeList();
    }
    else
    {
        LoggerDialog::get()->Error("SettingsEditorForm::processCommand","Unsuported command: " + command);
    }
}

void SettingsEditorForm::processListResp(QJsonObject &obj)
{
    QString resource = obj["Resource"].toString();
    QJsonObject data = obj["Data"].toObject();
    QJsonArray resArr = data["Resources"].toArray();
    QJsonArray resDirsArr = data["ResourcesDirs"].toArray();

    QStringList resList;
    QStringList resDirsList;

    for(int i=0;i<resArr.count(); i++)
        resList.append( resArr.at(i).toString());

    for(int i=0;i<resDirsArr.count(); i++)
        resDirsList.append( resDirsArr.at(i).toString());

    updateTreeList(resource, resDirsList, resList);
    requestNewLists(resource, resDirsList);
}

void SettingsEditorForm::processGetResp(QJsonObject &obj)
{
    QString resource = obj["Resource"].toString();
    QJsonObject data = obj["Data"].toObject();

    QJsonDocument doc(data);
    QString dataString = doc.toJson(QJsonDocument::Indented);
    ui->textEdit->setText( dataString );
    ui->lineEdit_res_path->setText( resource );
}

QString SettingsEditorForm::composeListCommand(QString dir)
{
    return QString("{\"Command\":\"List\", \"Resource\":\"%1\"}").arg(dir);
}

QString SettingsEditorForm::composeGetCommand(QString res)
{
    return QString("{\"Command\":\"Get\", \"Resource\":\"%1\"}").arg(res);
}

QString SettingsEditorForm::composeSetCommand(QString res, QString data)
{
    return QString("{\"Command\":\"Set\", \"Resource\":\"%1\",\"Data\":%2}").arg(res).arg(data);
}

QString SettingsEditorForm::composeRemoveCommand(QString res)
{
    return QString("{\"Command\":\"Remove\",\"Resource\":\"%1\"}").arg(res);
}


void SettingsEditorForm::trySendData(QByteArray arr)
{
    if(_socket == nullptr) return;
    if(_socket->write( arr ) == -1)
    {
        ui->pushButton_connect->setText("Connect");
        _socket->close();
    }
}

void SettingsEditorForm::trySendData(QString arr)
{
    QByteArray data; data.append(arr);
    trySendData(data);
}

void SettingsEditorForm::updateTreeList(QString path, QStringList newDirs, QStringList newElements)
{
    QStringList dirs = path.split("/");
    dirs.removeFirst();
    QTreeWidgetItem* iter = ui->treeWidget->topLevelItem(0);
    for(QString dir : dirs)
    {
        for(int i=0; i<iter->childCount(); i++)
        {
            if(iter->child(i)->type() == TWT_DIR
                    && iter->child(i)->text(0) == dir)
            {
                iter = iter->child(i);
            }
        }
    }

    for(QString dir : newDirs)
    {
        QTreeWidgetItem * newItem = new QTreeWidgetItem(TWT_DIR);
        newItem->setText(0, dir);
        iter->addChild( newItem );
    }

    for(QString element : newElements)
    {
        QTreeWidgetItem * newItem = new QTreeWidgetItem(TWT_RES);
        newItem->setText(0, element);
        iter->addChild( newItem );
    }

    ui->treeWidget->expandAll();
}

void SettingsEditorForm::requestNewLists(QString path, QStringList newDirs)
{
    for(QString dir : newDirs)
        trySendData(composeListCommand(QString("%1/%2").arg(path).arg(dir)));
}

void SettingsEditorForm::reloadTreeList()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("Resources");
    QTreeWidgetItem * rootItem = new QTreeWidgetItem(ui->treeWidget,TWT_DIR);
    rootItem->setText(0,".");

    trySendData(  composeListCommand(".") );
}

void SettingsEditorForm::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->type() == TWT_DIR) return;

    QString res;
    QTreeWidgetItem * iter = item;
    while(true)
    {
        res = iter->text(0) + res;
        iter = iter->parent();
        if(iter == nullptr) break;
        res = "/" + res;
    }

    trySendData(composeGetCommand(res));
}
