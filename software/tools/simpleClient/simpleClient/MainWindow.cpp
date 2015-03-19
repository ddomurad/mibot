#include "MainWindow.h"
#include <QTextCursor>

#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_connect_clicked()
{
    if(socket != nullptr)
    {
        socket->disconnectFromHost();
        socket->close();
        delete socket;
        socket = nullptr;
        ui->btn_connect->setText("Connect");

        return;
    }

    socket = new QSslSocket(this);

    connect( socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect( socket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(onPeerVerifyError(QSslError)) );
    connect( socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
    connect( socket, SIGNAL(connected()),this,SLOT(onConnected()));
    connect( socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect( socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    QString cert = ui->line_cert->text();

    socket->setLocalCertificate( "/home/work/Tmp/ssl_key/" + cert + ".crt" );
    socket->setPrivateKey( "/home/work/Tmp/ssl_key/" + cert + ".key", QSsl::Rsa, QSsl::Pem, ui->line_pass->text().toLatin1());

    socket->setProtocol(QSsl::SslV3);

    socket->setCaCertificates(QSslCertificate::fromPath("/home/work/Tmp/ssl_key/s1.pem"));

    socket->connectToHostEncrypted(
                ui->line_host->text(),
                ui->line_port->text().toInt(),
                 ui->line_peer->text());
}

void MainWindow::on_btn_send_clicked()
{
    ui->text_outcomming->textCursor().select(QTextCursor::Document);
    socket->write( ui->text_outcomming->toPlainText().toLatin1() );
}


void MainWindow::onSocketError(QAbstractSocket::SocketError e)
{
    Log("ERR", QString("Socket error (%1)").arg( ((QSslSocket*)sender())->errorString() ));

    ui->btn_connect->setText("Connect");
}

void MainWindow::onPeerVerifyError(QSslError err)
{
    Log("ERR", QString("PeerVerifyError (%1)").arg(err.errorString()));

    ui->btn_connect->setText("Connect");
}

void MainWindow::onSslErrors(QList<QSslError> errs)
{
    for(QSslError err : errs  )
        Log("ERR", QString("SslErrors (%1)").arg(err.errorString()));

    ui->btn_connect->setText("Connect");
}

void MainWindow::onConnected()
{
    Log("INF", "Connected" );
    ui->btn_connect->setText("Disconnect");
}

void MainWindow::onReadyRead()
{
    QByteArray arr =  socket->readAll();
    ui->text_incomming->clear();
    QTextCursor cursor(ui->text_incomming->document());
    cursor.insertText( QString(arr) );
}

void MainWindow::onBytesWritten(qint64 i)
{
    Log("INF", QString("Bytes writen: %1").arg(i) );
}

void MainWindow::Log(QString type, QString msg)
{
    QString logStr = "[" + type + "]: " + msg + "\n";
    QTextCursor cursor(ui->text_log->document());
    cursor.insertText(logStr);
    qDebug() << logStr;
}

