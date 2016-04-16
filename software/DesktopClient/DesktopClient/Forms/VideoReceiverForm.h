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
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
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

    static void onStaticData(QByteArray &data);
    void onData(QByteArray data);

private slots:
    void on_pushButton_server_Start_clicked();
    void on_pushButton_server_stop_clicked();

    void onConnection();
    void onDisconnection();
    void onSocketError(QAbstractSocket::SocketError);
    void onData();
    void onShutdownTimer();
    void onFpsTimer();
    void on_pushButton_select_store_file_clicked();
    void on_checkBox_store_toggled(bool checked);

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
    AVFrame * pictureRGB;
    unsigned char * rgbBuffer;
    AVCodecParserContext * parser;

    SwsContext *swsContext;

    void createSwsContext();
    void startDecoder();
    void stopDecoder();
    void updateDecoder();
    void procDecoder(u_int8_t * data, int size);
    void updateFrame(AVPacket *packet);
    void displayPicture();

    void storeFrames(QByteArray *data);
    bool _store_frames;

    static VideoReceiverForm * _self;
};

#endif // VIDEORECEIVERFORM_H
