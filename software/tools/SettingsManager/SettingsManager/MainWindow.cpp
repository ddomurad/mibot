#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>

#define TWT_DIR 0x01
#define TWT_RES 0x02

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _socket = nullptr;
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);

    QTreeWidgetItem * rootItem = new QTreeWidgetItem(ui->treeWidget,TWT_DIR);
    rootItem->setText(0,".");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSocketError(QAbstractSocket::SocketError)
{
    Log("ERR", QString("Socket error (%1)").arg( ((QSslSocket*)sender())->errorString() ));
    _socket->close();
    ui->pushButton_connect_to_server->setText("Connect");
}

void MainWindow::onPeerVerifyError(QSslError err)
{
    Log("ERR", QString("PeerVerifyError (%1)").arg(err.errorString()));
    _socket->close();
    ui->pushButton_connect_to_server->setText("Connect");
}

void MainWindow::onSslErrors(QList<QSslError> errs)
{
    for(QSslError err : errs  )
        Log("ERR", QString("SslErrors (%1)").arg(err.errorString()));
    _socket->close();
    ui->pushButton_connect_to_server->setText("Connect");
}

void MainWindow::onConnected()
{
    Log("INF", "Connected" );
    ui->pushButton_connect_to_server->setText("Disconnect");
}

void MainWindow::on_pushButton_connect_to_server_clicked()
{
    if(ui->pushButton_connect_to_server->text() == "Connect")
        connectToHost(ui->lineEdit_server_addr->text(), ui->spinBox_server_port->value());
    else
        disonnectFromHost();
}

void MainWindow::onReadyRead()
{
    QByteArray data = _socket->readAll();
    _protocol.PushData(data);
    fixProtocol();
    processProtocol();
}

void MainWindow::Log(QString typ, QString message)
{
    qDebug() << QString("[%1]: %2").arg(typ).arg(message);
}

void MainWindow::connectToHost(QString addr, int port)
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
        Log("ERR", "Wait for encrypted failed!");
        _socket->close();
        ui->pushButton_connect_to_server->setText("Connect");
    }
}

void MainWindow::disonnectFromHost()
{
    if(_socket == nullptr) return;
    _socket->close();
    ui->pushButton_connect_to_server->setText("Connect");
}

void MainWindow::fixProtocol()
{
    if(_protocol.IsDataCorupted())
    {
        Log("WRN", "Cleaning protocol corupted data");
        _protocol.RemoveCoruptedData();
        if(_protocol.IsDataCorupted())
        {
            Log("WRN", "Can't clean corupted data. Removing everythig.");
            _protocol.RemoveAllData();
        }
    }
}

void MainWindow::processProtocol()
{
    while(_protocol.ContainsValidJSONObject())
    {
        QJsonObject command = _protocol.GetPendingObject();
        processCommand( command );
        fixProtocol();
    }
}

void MainWindow::processCommand(QJsonObject &obj)
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
        Log("ERR", "Unsuported command: " + command);
    }
}

void MainWindow::processListResp(QJsonObject &obj)
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

void MainWindow::processGetResp(QJsonObject &obj)
{
    QString resource = obj["Resource"].toString();
    QJsonObject data = obj["Data"].toObject();

    QJsonDocument doc(data);
    QString dataString = doc.toJson(QJsonDocument::Indented);
    ui->textEdit_json_content->setText( dataString );
    ui->lineEdit_json_res->setText( resource );
}

QString MainWindow::composeListCommand(QString dir)
{
    return QString("{\"Command\":\"List\", \"Resource\":\"%1\"}").arg(dir);
}

QString MainWindow::composeGetCommand(QString res)
{
    return QString("{\"Command\":\"Get\", \"Resource\":\"%1\"}").arg(res);
}

QString MainWindow::composeSetCommand(QString res, QString data)
{
    return QString("{\"Command\":\"Set\", \"Resource\":\"%1\",\"Data\":%2}").arg(res).arg(data);
}

QString MainWindow::composeRemoveCommand(QString res)
{
    return QString("{\"Command\":\"Remove\",\"Resource\":\"%1\"}").arg(res);
}

void MainWindow::trySendData(QString arr)
{
    QByteArray data; data.append(arr);
    trySendData(data);
}

void MainWindow::updateTreeList(QString path, QStringList newDirs, QStringList newElements)
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

void MainWindow::requestNewLists(QString path, QStringList newDirs)
{
    for(QString dir : newDirs)
        trySendData(composeListCommand(QString("%1/%2").arg(path).arg(dir)));
}

void MainWindow::reloadTreeList()
{
    ui->treeWidget->clear();
    ui->treeWidget->setHeaderLabel("Resources");
    QTreeWidgetItem * rootItem = new QTreeWidgetItem(ui->treeWidget,TWT_DIR);
    rootItem->setText(0,".");

    trySendData(  composeListCommand(".") );
}

void MainWindow::trySendData(QByteArray arr)
{
    if(_socket == nullptr) return;
    if(_socket->write( arr ) == -1)
    {
        ui->pushButton_connect_to_server->setText("Connect");
        _socket->close();
    }
}

void MainWindow::on_pushButton_refreshdir_list_clicked()
{
    reloadTreeList();
}

void MainWindow::on_pushButton_explode_clicked()
{
    ui->treeWidget->expandAll();
}

void MainWindow::on_pushButton_colapse_clicked()
{
    ui->treeWidget->collapseAll();
}

void MainWindow::on_treeWidget_doubleClicked(const QModelIndex &index)
{
}

void MainWindow::on_pushButton_load_json_clicked()
{
    trySendData(composeGetCommand(ui->lineEdit_json_res->text()));
}

void MainWindow::on_pushButton_save_json_clicked()
{
    trySendData(composeSetCommand(ui->lineEdit_json_res->text(), ui->textEdit_json_content->toPlainText()));
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
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

void MainWindow::on_pushButton_remove_clicked()
{
    trySendData( composeRemoveCommand( ui->lineEdit_json_res->text() ));
}
