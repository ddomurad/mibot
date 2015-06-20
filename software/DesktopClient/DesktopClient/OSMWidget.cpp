#include "OSMWidget.h"
#include  <QBoxLayout>
#include <QGraphicsSceneMouseEvent>

double long2tilex(double lon, int z);
double lat2tiley(double lat, int z);
double tilex2long(double x, double z);
double tiley2lat(double y, double z);

OSMResourceDesc::OSMResourceDesc()
{}

OSMResourceDesc::OSMResourceDesc(QString osmUrl, QString cacheDir, QString format, QString errorTile) :
    _osmUrl(osmUrl),_cacheDir(cacheDir),
    _tileFormat(format), _errorTileFilePath(errorTile)
{
    if(_osmUrl.endsWith("/"))
        _osmUrl = _osmUrl.left(_osmUrl.length() - 1);

    if(!_cacheDir.endsWith("/"))
        _cacheDir.append("/");

    _cacheSubDir = osmUrl;
    _cacheSubDir.replace("/","_").replace(':',"");

    if(_cacheSubDir.endsWith("_"))
        _cacheSubDir = _cacheSubDir.left(_cacheSubDir.length() - 1);

    _cacheSubDir.append("/");
}

OSMResourceDesc::~OSMResourceDesc()
{}

QString OSMResourceDesc::getTileUrl(OSMTileDesc &t) const
{
    return QString("%1/%2/%3/%4.%5")
            .arg(_osmUrl)
            .arg(t.iz).arg(t.ix).arg(t.iy)
            .arg(_tileFormat);
}

QString OSMResourceDesc::getCachedTileFileName(OSMTileDesc &t) const
{
    return QString("tile_%1_%2_%3.%4")
            .arg(t.iz).arg(t.ix).arg(t.iy)
            .arg(_tileFormat);
}

QString OSMResourceDesc::getCachedTileFilePath(OSMTileDesc &t) const
{
    return _cacheDir + _cacheSubDir + getCachedTileFileName(t);
}

QString OSMResourceDesc::getCachedTileFileDir() const
{
    return _cacheDir + _cacheSubDir.left(_cacheSubDir.length() - 1);
}

QString OSMResourceDesc::getErrorTileFilePath() const
{
    return _errorTileFilePath;
}

bool OSMResourceDesc::tileFileExists(OSMTileDesc &t)
{
    return QFile(getCachedTileFilePath(t)).exists();
}
QString OSMResourceDesc::cacheDir() const
{
    return _cacheDir;
}

QString OSMResourceDesc::cacheSubDir() const
{
    return _cacheSubDir;
}

void OSMResourceDesc::Setup(OSMResourceDesc &d)
{
    _osmUrl = d._osmUrl;
    _cacheDir = d._cacheDir;
    _cacheSubDir = d._cacheSubDir;
    _tileFormat = d._tileFormat;
    _errorTileFilePath = d._errorTileFilePath;
}


OSMTileDesc::OSMTileDesc() :
    ix(0), iy(0), iz(0)
{}

OSMTileDesc::OSMTileDesc(int x, int y, int z) :
    ix(x), iy(y), iz(z)
{}

OSMTileItem::OSMTileItem(OSMTileDesc &t, OSMResourceDesc *r, QGraphicsItem *parent):
    QGraphicsItem(parent),
    _isValid(false),
    _resDesc(r),_tileDesc(t)
{}

OSMTileItem::~OSMTileItem()
{}

QRectF OSMTileItem::boundingRect() const
{
    return QRectF(0,0,1,1);
}

void OSMTileItem::paint(QPainter *painter,const QStyleOptionGraphicsItem *,QWidget *)
{
    updateTile(false);
    painter->drawPixmap(0,0,1,1,_pixmap);
}

void OSMTileItem::updateTile(bool force)
{
    setPos( _tileDesc.ix, _tileDesc.iy);

    if(!force && _isValid) return;

    QPixmap new_pixmap = QPixmap( _resDesc->getCachedTileFilePath(_tileDesc) );
    if(new_pixmap.isNull())
    {
        if(!_isValid && !_pixmap.isNull() && !force) return;

        _pixmap = QPixmap(_resDesc->getErrorTileFilePath());
        if(_pixmap.isNull())
        {
            _pixmap = QPixmap(256,256);
            _pixmap.fill( Qt::red );
        }
        _isValid = false;
    }else
    {
        _pixmap = new_pixmap;
        _isValid = true;
    }

    update(boundingRect());
}

bool OSMTileItem::isValid() { return _isValid; }
QPixmap OSMTileItem::pixmap() { return _pixmap; }

OSMTileDesc OSMTileItem::tileDesc() { return _tileDesc; }


OSMScene::OSMScene(OSMResourceDesc * r, QObject *parent)
    : QGraphicsScene(parent),
      _resDesc(r),
      _iz(0),
      _enableScrollZoom(true),
      _secret_item(nullptr),
      _drag(false),
      _translation(QPointF(0,0))
{}

void OSMScene::updateTile(OSMTileDesc t)
{
    OSMTileItem * tileItem = getTileItem(t);

    if(tileItem != nullptr)
    {
        tileItem->updateTile(true);
        if(!tileItem->isValid())
            emit downlaodTile(tileItem->tileDesc());
    }
    else
    {
        addTile(t);
    }
}

OSMTileItem *OSMScene::getTileItem(OSMTileDesc &t)
{
    for(QGraphicsItem * item : items())
    {
        OSMTileItem* tileItem = dynamic_cast<OSMTileItem*>(item);
        if(tileItem == nullptr) continue;
        OSMTileDesc tmpt = tileItem->tileDesc();

        if(tmpt.ix == t.ix && tmpt.iy == t.iy && tmpt.iz == t.iz)
        {return tileItem;}
    }

    return nullptr;
}

void OSMScene::addTile(OSMTileDesc &t)
{
    OSMTileItem * item = new OSMTileItem(t,_resDesc);
    this->addItem(item);
    item->updateTile(true);
    if(!item->isValid())
        emit downlaodTile(item->tileDesc());
}

void OSMScene::updateScene()
{
    QList<QGraphicsView*> views = this->views();
    QGraphicsView* gv = views[0];

    if(_secret_item == nullptr)
    {
        _secret_item = addRect(0,0,_translation.x(),_translation.y());
        _secret_item->setOpacity(0.1);
    }
    else
    {
        _secret_item->setRect(0,0,_translation.x(),_translation.y());
    }

    QPoint p1 = gv->mapToScene(0, 0).toPoint();
    QPoint p2 = gv->mapToScene(gv->width(), gv->height()).toPoint() ;//+ QPoint(1,1);

    QList<QGraphicsItem*> visible_items = items(p1.x(),p1.y(), p2.x() - p1.x(), p2.y() - p1.y(),
                                                        Qt::IntersectsItemShape,Qt::AscendingOrder);
    QList<QGraphicsItem*> all_items = items();

    for(int i=all_items.size()-1;i>=0;i--)
    {
        if(all_items[i] == _secret_item) continue;
        if(!visible_items.contains( all_items[i] ))
        {
            removeItem(all_items[i]);
            delete all_items[i];
        }
    }

    int x = p1.x();
    int y = p1.y();
    int w = p2.x() - p1.x();
    int h = p2.y() - p1.y();

    for(int ix = 0; ix < w; ix++)
    {
        for(int iy = 0; iy<h; iy++)
        {
            updateTile(OSMTileDesc(x + ix, y + iy, _iz));
        }
    }
}

void OSMScene::setZoom(int z)
{
    _iz = z;
}

void OSMScene::focusGPSPoint(QPointF p)
{
    double x = long2tilex(p.x(),_iz);
    double y = lat2tiley(p.y(), _iz);
    _translation = QPointF(x,y);
    updateScene();
}

void OSMScene::enableScrollZoom(bool enable)
{
    _enableScrollZoom = enable;
}

void OSMScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton)
        updateScene();
}

void OSMScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->delta() > 0)
        _iz ++;
    else if(event->delta() < 0)
        _iz --;

    if(_iz <0) _iz = 0;
    if(_iz > 19 ) _iz = 19;

    updateScene();
}

void OSMScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}


OSMTileDownloader::OSMTileDownloader(OSMResourceDesc *resource, QObject *parent) :
    QObject(parent),
    _resDesc(resource)
{
    _accessManager = new QNetworkAccessManager(this);
}

OSMTileDownloader::~OSMTileDownloader()
{}

void OSMTileDownloader::downlaodTile(OSMTileDesc  trd)
{
    if(isWaitingForTile(trd)) return;
    qDebug() << "downlaoding new tile: " << trd.ix << trd.iy << trd.iz;

    _getsWaiting.append(trd);
    QNetworkRequest request( _resDesc->getTileUrl(trd) );
    QNetworkReply * reply = _accessManager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(onTileDownloaded()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onTileDownloadingError(QNetworkReply::NetworkError)));
}

bool OSMTileDownloader::isWaitingForTile(OSMTileDesc &trd)
{
    QString tmp_url = _resDesc->getTileUrl(trd);
    QString tmp_file = _resDesc->getCachedTileFilePath(trd);

    for(OSMTileDesc getInfo : _getsWaiting)
    {
        if(_resDesc->getTileUrl(getInfo) == tmp_url
                && _resDesc->getCachedTileFilePath(getInfo) == tmp_file)
            return true;
    }
    return false;
}

int OSMTileDownloader::waitingRequests()
{
    return _getsWaiting.length();
}

void OSMTileDownloader::onTileDownloaded()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    QNetworkRequest request = reply->request();

    OSMTileDesc tileDesc;
    QByteArray tile_data = reply->readAll();
    reply->deleteLater();
    if(!getAndRemoveFromListCorrespondingGetRequest(request, &tileDesc))
        return;

    QString newTileFilePath = _resDesc->getCachedTileFilePath(tileDesc);

    QFile tileFile(newTileFilePath);

    tryAddMissingDirectories();

    if(!tileFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can't write tile filet to: " << newTileFilePath;
    }else
    {
        tileFile.write( tile_data );
        tileFile.close();
    }

    emit tileDownloaded(tileDesc);

    if(_getsWaiting.length() == 0)
        emit allTilesDownloaded();
}

void OSMTileDownloader::onTileDownloadingError(QNetworkReply::NetworkError e)
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    QNetworkRequest request = reply->request();
    OSMTileDesc tileDesc;

    qDebug() << QString("Tile downloading error: %1, url: %2")
                .arg(e)
                .arg(request.url().toString());

    reply->deleteLater();

    if(!getAndRemoveFromListCorrespondingGetRequest(request, &tileDesc))
        return;

    copyErrorTileAsNewTile(tileDesc);

    emit tileDownloaded(tileDesc);

    if(_getsWaiting.length() == 0)
        emit allTilesDownloaded();
}

bool OSMTileDownloader::getAndRemoveFromListCorrespondingGetRequest(QNetworkRequest &req, OSMTileDesc *getReqInfo)
{
    for(int i=0; i<_getsWaiting.length(); i++)
    {
        //if(_getsWaiting[i].isCorresponding(req))
        if(_resDesc->getTileUrl(_getsWaiting[i]) == req.url().toString())
        {
            *getReqInfo = _getsWaiting[i];
            _getsWaiting.removeAt(i);
            return true;
        }
    }

    return false;
}

void OSMTileDownloader::copyErrorTileAsNewTile(OSMTileDesc &trd)
{
    tryAddMissingDirectories();
    QFile::copy(_resDesc->getErrorTileFilePath(),_resDesc->getCachedTileFilePath(trd));
}

void OSMTileDownloader::tryAddMissingDirectories()
{
    QDir cacheDir(_resDesc->getCachedTileFileDir());
    if(!cacheDir.exists())
    {
        cacheDir = QDir(_resDesc->cacheDir());
        if(!cacheDir.exists())
        {
            qDebug() << "Cache directory dont exits: " << _resDesc->cacheDir();
        }else
        {
            if(!cacheDir.mkdir( _resDesc->cacheSubDir() ))
                qDebug() << "Can't create subdirectory for cache: " << _resDesc->getCachedTileFileDir();
        }
    }
}

OSMWidget::OSMWidget(QWidget *parent) :
    QWidget(parent)
{
    _resourceDesc = new OSMResourceDesc();

    _tileDownloader = new OSMTileDownloader(_resourceDesc,this);

    connect(_tileDownloader, SIGNAL(tileDownloaded(OSMTileDesc)),
             this,SLOT(onTileDownloaded(OSMTileDesc)));

    connect(_tileDownloader, SIGNAL(allTilesDownloaded()),
            this, SLOT(onAllTilesDownladed()));


    _graphicsView = new QGraphicsView(this);
    QBoxLayout * layout = new QBoxLayout(QBoxLayout::TopToBottom, this);
    layout->addWidget(_graphicsView);
    this->setLayout(layout);

    _scene = new OSMScene(_resourceDesc, this);
    _graphicsView->setScene(_scene);
    _graphicsView->scale(256,256);
    _graphicsView->setMouseTracking(true);
    _graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    _graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _graphicsView->setInteractive(true);
    //_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    connect(_tileDownloader, SIGNAL(tileDownloaded(OSMTileDesc)),
            _scene,SLOT(updateTile(OSMTileDesc)));

    connect(_scene, SIGNAL(downlaodTile(OSMTileDesc)),
            _tileDownloader, SLOT(downlaodTile(OSMTileDesc)));
}

OSMWidget::~OSMWidget()
{
    delete _resourceDesc;
}

void OSMWidget::setResourcesDesc(OSMResourceDesc desc)
{
    _resourceDesc->Setup(desc);
}

void OSMWidget::setZoom(int z)
{
    _scene->setZoom(z);
}

void OSMWidget::setZoomRange(int minZoom, int maxZoom)
{

}



void OSMWidget::focusPoint(QPointF p)
{
    _scene->focusGPSPoint(p);
}

void OSMWidget::enableScrollZoom(bool enable)
{
    _scene->enableScrollZoom(enable);
}

void OSMWidget::onTileDownloaded(OSMTileDesc t)
{
    pedingTilesCountChanged( _tileDownloader->waitingRequests() );
}

void OSMWidget::onAllTilesDownladed()
{
    emit pedingTilesCountChanged(0);
}



double long2tilex(double lon, int z)
{
    return (lon + 180.0) / 360.0 * pow(2.0, z);
}

double lat2tiley(double lat, int z)
{
    return (1.0 - log( tan(lat * M_PI/180.0) + 1.0 / cos(lat * M_PI/180.0)) / M_PI) / 2.0 * pow(2.0, z);
}

double tilex2long(double x, double z)
{
    return x / pow(2.0, z) * 360.0 - 180.0;
}

double tiley2lat(double y, double z)
{
    double n = M_PI - 2.0 * M_PI * y / pow(2.0, z);
    return 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
}
