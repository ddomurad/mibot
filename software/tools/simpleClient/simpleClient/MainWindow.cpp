#include "MainWindow.h"
#include <QTextCursor>
#include <QMessageBox>
#include "ui_MainWindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(nullptr)
{
    ui->setupUi(this);
    send_type = 0;
    js_input_thread = new JsInput(this);

    send_timer = new QTimer(this);
    connect( send_timer, SIGNAL(timeout()), this, SLOT(onSendTimer()));
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

    QSslSocket * sslSocket = new QSslSocket(this);

    socket = sslSocket;
    connect( socket, SIGNAL(error(QAbstractSocket::SocketError)), this,SLOT(onSocketError(QAbstractSocket::SocketError)));
    if(ui->check_enable_ssl->isChecked())
    {
        connect( sslSocket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(onPeerVerifyError(QSslError)) );
        connect( sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
    }
    connect( socket, SIGNAL(connected()),this,SLOT(onConnected()));
    connect( socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect( socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    QString cert = ui->line_cert->text();

    if(ui->check_enable_ssl->isChecked())
    {
        sslSocket->setLocalCertificate( "/home/work/Tmp/cert/" + cert + "/c.crt" );
        sslSocket->setPrivateKey( "/home/work/Tmp/cert/" + cert + "/c.key", QSsl::Rsa, QSsl::Pem, ui->line_pass->text().toLatin1());

        sslSocket->setProtocol(QSsl::SslV3);

        sslSocket->setCaCertificates(QSslCertificate::fromPath("/home/work/Tmp/cert/s.pem"));

        sslSocket->connectToHostEncrypted(
                    ui->line_host->text(),
                    ui->line_port->text().toInt(),
                    ui->line_peer->text());
    }else
    {
        socket->connectToHost(ui->line_host->text(),
                              ui->line_port->text().toInt());
    }
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

    if( ui->combo_disp_inp->currentText() == "Hex" )
    {
        QString str;
        for( int i=0; i< arr.length(); i++ )
        {
            str +=  QString::number( (int) (arr.at(i)) & 0x000000ff, 16 ) ;
            str += " ";
        }

        ui->incomming->append(str);

    }else
    {
        ui->incomming->append( QString( arr ) );
    }

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

void MainWindow::stopSending()
{
    ui->cb_js_run->setChecked( false );
}

void MainWindow::sendJsState()
{
    int ax_l = ui->left_ax->value();
    int ax_r = ui->right_ax->value();
    int brake_btn = ui->sb_brake_button->value();

    bool rl = ui->cb_js_reverse_left->isChecked();
    bool rr = ui->cb_js_reverse_right->isChecked();

    int left_ax_val = js_input_thread->axis[ax_l] * (rl ? -1 :1);
    int right_ax_val = js_input_thread->axis[ax_r] * (rr ? -1 :1);


    bool brake_state = js_input_thread->btn[brake_btn];

    ui->js_left_read->setText( QString::number( left_ax_val ) );
    ui->js_right_read->setText( QString::number( right_ax_val ) );

    ui->lax->setValue( left_ax_val );
    ui->rax->setValue( right_ax_val );

    ui->rb_brake->setChecked( brake_state );

    char left  =   100 * ( float(left_ax_val)/32767.0f );
    char right =   100 * ( float(right_ax_val)/32767.0f );
    bool is_sim = ui->cb_is_simulation->isChecked();
    uchar data[] =
    {
        0x03, // write cmd
        0x00, // addr
         // driver model ( 0000 100S )
        0x08
            | ( is_sim ? 0x01 : 0x00)
            | ( brake_state ? 0x04 : 0x00 ),
        left, right
    };

    if(socket == nullptr) return;
    if(!socket->isOpen()) return;

    socket->write( (char*)data, 0x05 );

    QString hex_txt = "[";

    for(int i=0;i<0x05;i++)
    {
        hex_txt += QString::number( (int)data[i],16);
        if(i != 0x04) hex_txt += " ";
    }

    hex_txt += "]";

    ui->out_plain_text->append( hex_txt );

    if(ui->cb_Query_Stsatus->isChecked())
    {
        uchar qdata[] =
        {
            0x30, // write cmd
            0x00 // addr
        };

        if(socket == nullptr) return;
        if(!socket->isOpen()) return;

        socket->write( (char*)qdata, 0x02 );
    }
}

void MainWindow::on_btn_Check_js_clicked()
{
    QString js_file = ui->le_js_file->text();
    QFile file(js_file);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical( this, "Joystick test.","Joystic input file opening error.");
    }else
    {
        QMessageBox::information( this, "Joystick test.","Joystic input file opening sucess.");
        file.close();
    }
}

void MainWindow::on_cb_js_run_toggled(bool checked)
{
    if(checked)
    {
        if(send_timer->isActive()) stopSending();
        js_input_thread->Start( ui->le_js_file->text() );

        send_type = 2;
        send_timer->start( ui->sb_js_send_Ratio->value() );
    }
    else
    {
        js_input_thread->Stop();
        send_timer->stop();
    }
}

void MainWindow::onSendTimer()
{
    if(send_type == 2)
    { sendJsState(); return; }

    stopSending();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->out_plain_text->clear();
}

void MainWindow::on_pushButton_clicked()
{
    ui->incomming->clear();
}
