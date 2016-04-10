#include "VideoReceiverForm.h"
#include "ui_VideoReceiverForm.h"
#include <QHostAddress>
#include <QScrollBar>
#include <QFileDialog>
#include "Other/Recorder.h"

#define SHC if(!_shall_run) return;
#define DATA_MAX_SIZE 4096
#define CHECK(X) if(!X) {ELog(QString("%1 is null").arg(#X)); return; }

VideoReceiverForm::VideoReceiverForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoReceiverForm)
{
    _shall_run = false;
    _tcp_client = nullptr;

    ui->setupUi(this);
    ui->label_server_state->setText("Stop");

    _tcp_server = new QTcpServer(this);
    connect(_tcp_server, SIGNAL(newConnection()), this, SLOT(onConnection()));

    _shutdown_timer = new QTimer(this);
    connect(_shutdown_timer, SIGNAL(timeout()), this, SLOT(onShutdownTimer()));

    _fps_timer = new QTimer(this);
    connect(_fps_timer, SIGNAL(timeout()), this, SLOT(onFpsTimer()));
    _fps_timer->start(1000);

    _fps_frames = 0;
    _is_decoder_started = false;

    _store_frames = false;

    codec = nullptr;
    context = nullptr;
    pictureRGB = nullptr;
    rgbBuffer = nullptr;
    swsContext = nullptr;
    picture = nullptr;
    parser = nullptr;
    _self = this;
}

VideoReceiverForm::~VideoReceiverForm()
{
    _self = nullptr;
    delete ui;
    stopDecoder();
}

void VideoReceiverForm::onStaticData(QByteArray &data)
{
    if(_self != nullptr)
    {
        _self->_shall_run = true;
        _self->onData(data);
    }
}

void VideoReceiverForm::onData(QByteArray data)
{
    storeFrames(&data);

    _video_data_buffer.append(data);
    if(!_is_decoder_started)
        startDecoder();

    int cache_limit = ui->spinBox_buffer->value();
    ui->label_act_buffer_size->setText(QString::number(_video_data_buffer.size()));
    if(_video_data_buffer.size() > cache_limit)
        updateDecoder();
    else
        Log("INFO", "cache limit not achived");
}

void VideoReceiverForm::on_pushButton_server_Start_clicked()
{
    _shall_run = true;
    if(!_tcp_server->listen(QHostAddress::Any,
                       ui->spinBox_server_port->value()))
        ELog("Listen error");
    else
        ILog("Listening ...");
}

void VideoReceiverForm::on_pushButton_server_stop_clicked()
{
    _shall_run = false;
    if(_tcp_client != nullptr)
    {
        _tcp_client->close();
        _tcp_client->deleteLater();
        _tcp_client = nullptr;
    }

    _tcp_server->close();
}

void VideoReceiverForm::onConnection()
{
    _shall_run = true;
    if(_tcp_client != nullptr)
    {
        WLog("Old connection closed.");
        _tcp_client->close();
        _tcp_client->deleteLater();
    }

    _tcp_client = _tcp_server->nextPendingConnection();
    connect(_tcp_client, SIGNAL(disconnected()), this, SLOT(onDisconnection()));
    connect(_tcp_client, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(_tcp_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    ILog("New connection");
}

void VideoReceiverForm::onDisconnection()
{
    _shutdown_timer->start(1);
}

void VideoReceiverForm::onSocketError(QAbstractSocket::SocketError)
{
    if(_tcp_client != nullptr)
    {
        ELog(_tcp_client->errorString());

        _tcp_client->close();
        _tcp_client->deleteLater();
        _tcp_client = nullptr;

        _shutdown_timer->start(1);
    }else
    {
        ELog("Socket error");
    }
}


void VideoReceiverForm::onData()
{
    if(!_shall_run)
    {
        _tcp_client->close();
        return;
    }

    QByteArray data = _tcp_client->readAll();
    Recorder::get()->WriteData("Video", data);
    onData(data);
}

void VideoReceiverForm::onShutdownTimer()
{
    static int last_b_size = 0;

    if(_is_decoder_started &&
            (_video_data_buffer.size() == 0 ||
             last_b_size == _video_data_buffer.size()))
    {
        stopDecoder();
        _shutdown_timer->stop();
    }
    else
    {
        last_b_size = _video_data_buffer.size();
        updateDecoder();
    }
}

void VideoReceiverForm::onFpsTimer()
{
    static int last_frames = 0;
    qreal d = (_fps_frames - last_frames);
    last_frames = _fps_frames;

    ui->label_fps->setText( QString::number(d) );
}

void VideoReceiverForm::Log(QString type, QString msg)
{
    ui->listWidget_log_list->addItem(  QString("%1: %2").arg(type,msg) );

    ui->listWidget_log_list->item(ui->listWidget_log_list->count() - 1)->setSelected(true);
    ui->listWidget_log_list->verticalScrollBar()->setValue(ui->listWidget_log_list->verticalScrollBar()->maximum());
}

void VideoReceiverForm::ILog(QString msg) { Log("INFO", msg); }
void VideoReceiverForm::WLog(QString msg) { Log("WARNING", msg); }
void VideoReceiverForm::ELog(QString msg) { Log("ERROR", msg); _shall_run = false; }

void VideoReceiverForm::createSwsContext()
{
    pictureRGB = avcodec_alloc_frame();
    int rgbPictureBytes = avpicture_get_size(PIX_FMT_RGB32, context->width, context->height);
    rgbBuffer = new unsigned char[rgbPictureBytes];
    avpicture_fill( (AVPicture*) pictureRGB, rgbBuffer, PIX_FMT_RGB32, context->width, context->height);

    swsContext = sws_getContext(context->width, context->height, context->pix_fmt,
                                context->width, context->height,PIX_FMT_RGB32, SWS_BILINEAR,
                                NULL,NULL, NULL);
}

void VideoReceiverForm::startDecoder()
{
    _fps_frames = 0;

    SHC;
    avcodec_register_all();
    codec = avcodec_find_decoder( CODEC_ID_H264 );
    CHECK(codec);

    context = avcodec_alloc_context3(codec);

    if(codec->capabilities & CODEC_CAP_TRUNCATED)
        context->flags |= CODEC_FLAG_TRUNCATED;

    if(avcodec_open2( context, codec, 0) < 0)
        ELog("Can't open avcodec !");

    SHC;

    picture = //av_frame_alloc();
        avcodec_alloc_frame();

    parser = av_parser_init(CODEC_ID_H264);

    CHECK(parser);

    _is_decoder_started = true;
}

void VideoReceiverForm::stopDecoder()
{
    if(pictureRGB)
    {
        av_free(pictureRGB);
        pictureRGB = nullptr;
    }

    if(rgbBuffer)
    {
        delete[] rgbBuffer;
        rgbBuffer = nullptr;
    }

    if(swsContext)
    {
        sws_freeContext(swsContext);
        swsContext = nullptr;
    }

    if(picture)
    {
        av_free(picture);
        picture = nullptr;
    }

    if(parser)
    {
        av_parser_close(parser);
        parser = nullptr;
    }

    if(context)
    {
        avcodec_close(context);
        av_free(context);
        context = nullptr;
    }

    _video_data_buffer.clear();
    _shutdown_timer->stop();

    _is_decoder_started = false;
}

void VideoReceiverForm::updateDecoder()
{
    if(_video_data_buffer.size() == 0)
        return;

    u_int8_t * data = 0;
    int size = 0;
    int len = av_parser_parse2( parser, context, &data,  &size,
                                (u_int8_t*)_video_data_buffer.constData(), _video_data_buffer.size(),
                                0,0, AV_NOPTS_VALUE );

    if(size == 0 && len >= 0)
        return;

    if(data != 0)
    {
        procDecoder((u_int8_t*) _video_data_buffer.constData(), size );
        ui->label_resolution->setText( QString("%1x%2").arg(picture->width).arg(picture->height) );
    }

    _video_data_buffer = _video_data_buffer.remove(0, size);
}

void VideoReceiverForm::procDecoder(u_int8_t *data, int size)
{
    AVPacket packet;
    int len = 0;
    int got_frame = 0;

    av_init_packet(&packet);

    packet.pts = AV_NOPTS_VALUE;
    packet.dts = AV_NOPTS_VALUE;
    packet.data = data;
    packet.size = size;

    len = avcodec_decode_video2( context, picture, &got_frame, &packet );

    if(len < 0)
    {
        WLog("Can't decode frame data !");
    }

    if(got_frame == 0)
        return;

    updateFrame(&packet);
}

void VideoReceiverForm::updateFrame(AVPacket *packet)
{
    _fps_frames++;
    ui->label_frame_cnt->setText( QString::number(_fps_frames, 10) );
    if(swsContext == nullptr)
        createSwsContext();

    sws_scale(swsContext, picture->data, picture->linesize, 0, picture->height,
              pictureRGB->data, pictureRGB->linesize);

    displayPicture();
}

void VideoReceiverForm::displayPicture()
{
    QImage image = QImage(rgbBuffer, context->width, context->height, QImage::Format_RGB32);
    ui->label_image->setPixmap(QPixmap::fromImage(image));
}

void VideoReceiverForm::storeFrames(QByteArray * data)
{
    if(!_store_frames)
        return;

    QFile of(ui->lineEdit_store_file_name->text());
    if(!of.open(QIODevice::Append))
    {
        ELog(QString("Cam't open file to append: '%1'").arg(ui->lineEdit_store_file_name->text()));
        ui->checkBox_store->setChecked(false);
        _store_frames = false;
    }
    of.write(*data);
    of.close();
}

void VideoReceiverForm::on_pushButton_select_store_file_clicked()
{
    ui->lineEdit_store_file_name->setText( QFileDialog::getSaveFileName(this, "Save video", QDir::currentPath()) );
}

void VideoReceiverForm::on_checkBox_store_toggled(bool checked)
{
    _store_frames = checked;
}

VideoReceiverForm *VideoReceiverForm::_self = nullptr;
