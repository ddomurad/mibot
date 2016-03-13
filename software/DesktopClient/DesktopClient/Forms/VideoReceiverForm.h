#ifndef VIDEORECEIVERFORM_H
#define VIDEORECEIVERFORM_H

#include <QWidget>
#include "ReferenceCounter.h"
#include <QtNetwork>
#include <QTimer>

extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavutil/avutil.h>
}


namespace Ui {
class VideoReceiverForm;
}

class VideoReceiverForm : public QWidget, public ReferenceCounter<VideoReceiverForm>
{
    Q_OBJECT

public:
    explicit VideoReceiverForm(QWidget *parent = 0);
    ~VideoReceiverForm();

private slots:
    void on_pushButton_server_Start_clicked();
    void on_pushButton_server_stop_clicked();
    void on_spinBox_buffer_valueChanged(int arg1);

    void onConnection();
    void onDisconnection();
    void onSocketError(QAbstractSocket::SocketError);
    void onData();
    void onShutdownTimer();
    void onFpsTimer();

private:
    Ui::VideoReceiverForm *ui;
    QTcpServer * _tcp_server;
    QTcpSocket * _tcp_client;
    QTimer * _shutdown_timer;
    QTimer * _fps_timer;

    bool _is_decoder_started;
    int _fps_frames;

    bool _shall_run;
    void Log(QString type, QString msg);
    void ILog(QString msg);
    void WLog(QString msg);
    void ELog(QString msg);

    QByteArray _video_data_buffer;

    // AVCODEC
    AVCodec * codec;
    AVCodecContext * context;
    AVFrame * picture;
    AVCodecParserContext * parser;

    void startDecoder();
    void stopDecoder();
    void updateDecoder();
    void procDecoder(u_int8_t * data, int size);
    void updateFrame(AVPacket *packet);
    void displayPicture();
};

#endif // VIDEORECEIVERFORM_H
