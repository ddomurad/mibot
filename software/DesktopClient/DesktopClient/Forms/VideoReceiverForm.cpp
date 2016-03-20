#include "VideoReceiverForm.h"
#include "ui_VideoReceiverForm.h"
#include <QHostAddress>
#include <QScrollBar>
#include <QFileDialog>

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
}

VideoReceiverForm::~VideoReceiverForm()
{
    delete ui;
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

void VideoReceiverForm::on_spinBox_buffer_valueChanged(int arg1)
{

}

void VideoReceiverForm::onConnection()
{
    if(_tcp_client != nullptr)
    {
        WLog("Old connection closed.");
        _tcp_client->close();
        _tcp_client->deleteLater();
    }

    _tcp_client = _tcp_server->nextPendingConnection();
    connect(_tcp_client, SIGNAL(disconnected()), this, SLOT(onDosconnection()));
    connect(_tcp_client, SIGNAL(readyRead()), this, SLOT(onData()));
    connect(_tcp_client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    ILog("New connection");
}

void VideoReceiverForm::onDisconnection()
{
    _shutdown_timer->start(10);
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
    storeFrames(&data);

    _video_data_buffer.append(data);
    if(!_is_decoder_started)
        startDecoder();

    int cache_limit = ui->spinBox_buffer->value();

    if(_video_data_buffer.size() > cache_limit)
        updateDecoder();
    else
        Log("INFO", "cache limit not achived");
}

void VideoReceiverForm::onShutdownTimer()
{
    static int last_b_size = 0;

    if(_is_decoder_started &&
            (_video_data_buffer.size() == 0 ||
             last_b_size == _video_data_buffer.size()))
    {
        stopDecoder();
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

void VideoReceiverForm::startDecoder()
{
    _fps_frames = 0;

    SHC;
    avcodec_register_all();
    codec = avcodec_find_decoder( AV_CODEC_ID_H264);
    CHECK(codec);

    context = avcodec_alloc_context3(codec);

    if(codec->capabilities & CODEC_CAP_TRUNCATED)
        context->flags |= CODEC_FLAG_TRUNCATED;

    if(avcodec_open2( context, codec, 0) < 0)
        ELog("Can't open avcodec !");

    SHC;

    picture = av_frame_alloc();
    parser = av_parser_init(AV_CODEC_ID_H264);

    CHECK(parser);

    _is_decoder_started = true;
}

void VideoReceiverForm::stopDecoder()
{
    if(parser)
    {
        av_parser_close(parser);
        parser = NULL;
    }

    if(context)
    {
        avcodec_close(context);
        av_free(context);
        context = NULL;
    }

    if(picture)
    {
        av_free(picture);
        picture = NULL;
    }

    _video_data_buffer.clear();
    _shutdown_timer->stop();
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

    procDecoder((u_int8_t*) _video_data_buffer.constData(), size );
    _video_data_buffer = _video_data_buffer.remove(0, size);
    ui->label_resolution->setText( QString("%1x%2").arg(picture->width).arg(picture->height) );
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
    displayPicture();
}

void VideoReceiverForm::displayPicture()
{
    static int iss=0;
    iss++;
    int skipN = ui->spinBox_skip->value();
    if(skipN != 0 && iss % skipN != 0) return;

    QImage image = QImage(picture->width, picture->height, QImage::Format_RGB32);


    for (int y = 0; y < picture->height; y++)
    {
        for (int x = 0; x < picture->width; x++)
        {
            const int xx = x >> 1;
            const int yy = y >> 1;
            const int Y = picture->data[0][y * picture->linesize[0] + x] - 16;
            const int U = picture->data[1][yy * picture->linesize[1] + xx] - 128;
            const int V = picture->data[2][yy * picture->linesize[2] + xx] - 128;
            const int r = qBound(0, (298 * Y + 409 * V + 128) >> 8, 255);
            const int g = qBound(0, (298 * Y - 100 * U - 208 * V + 128) >> 8, 255);
            const int b = qBound(0, (298 * Y + 516 * U + 128) >> 8, 255);

            image.setPixel(x, y, qRgb(r, g, b));
        }
    }

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
