#include "OSMWidget.h"

#include <QDir>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include "./Settings/AppSettings.h"

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


OSMWidget::OSMWidget(QWidget *parent) :
    QWidget(parent),
  _canMouseDrag(true),
  _canMouseZoom(true),
  _markerSelectEnable(true),
  _drag(false),_dragSpeed(1.0),
  _translate(QPointF(0,0)),
  _lastMousePos(QPointF(0,0)),
  _zoom(5),_minZoom(0),_maxZoom(18),
  _selectedMarker(nullptr),_selectedRoute(nullptr),
  _editidMarker(nullptr)
{
    _accessManager = new QNetworkAccessManager(this);
    _download_timer = new QTimer(this);

    connect(_download_timer, SIGNAL(timeout()), this, SLOT(onDownloadTimer()));
    _download_timer->start(1000);
    setTileSize(1.0);

    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)));
}

OSMWidget::~OSMWidget()
{}

void OSMWidget::onTileDownloaded()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    QNetworkRequest request = reply->request();
    QString request_url = request.url().toString();

    if(request_url.isEmpty()) return;
    QStringList splited = request_url.split('/');

    int zoom = splited[splited.size() - 3].toInt();
    int x = splited[splited.size() - 2].toInt();
    QString sy = splited[splited.size() - 1];
    int y = sy.split('.').at(0).toInt();

    QByteArray tile_data = reply->readAll();
    reply->deleteLater();

    QString newTileFilePath = getCachedTileFilePath(x, y, zoom);

    QFile tileFile(newTileFilePath);

    tryAddMissingDirectories();
    if(tile_data.startsWith("<!DOCTYPE html PUBLIC")
            || tile_data.startsWith("Sorry")
            || tile_data.startsWith("The service timed out"))
    {
        copyErrorTileAsNewTile(x,y,zoom);
        qDebug() << "There is no tile: " << request_url;
    }
    else
    {
        if(!tileFile.open(QIODevice::WriteOnly))
        {
            qDebug() << "Can't write tile filet to: " << newTileFilePath;
        }else
        {
            tileFile.write( tile_data );
            tileFile.close();
        }
    }

    emit tileDownloadingDone(toDownload());
    repaint();
    _waitingForRequestResp = "";
    _downlaodTile();
}

void OSMWidget::onTileDownloadingError(QNetworkReply::NetworkError e)
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    QNetworkRequest request = reply->request();
    QString request_url = request.url().toString();

    qDebug() << QString("Tile downloading error: %1, url: %2")
                .arg(e).arg(request_url);

    if(request_url.isEmpty()) return;
    QStringList splited = request_url.split('/');

    int zoom = splited[splited.size() - 3].toInt();
    int x = splited[splited.size() - 2].toInt();
    QString sy = splited[splited.size() - 1];
    int y = sy.split('.').at(0).toInt();

    reply->deleteLater();

    copyErrorTileAsNewTile(x,y,zoom);

    emit tileDownloadingDone(toDownload());
    repaint();
    _waitingForRequestResp = "";
    _downlaodTile();
}

void OSMWidget::onDownloadTimer()
{
    _waitingForRequestResp = "";
    _downlaodTile();
}

void OSMWidget::onContextMenu(const QPoint &pos)
{
    OSMMenuItem mitem;

    QPoint gpos = mapToGlobal(pos);
    QPointF gpsPos = getGPSPossition(pos);

    mitem.gpsPos = gpsPos;
    mitem.mouePos = pos;
    mitem.marker = _selectedMarker;
    mitem.route = _selectedRoute;

    QMenu menu;

    if(_selectedMarker != nullptr)
    {
        if(_markerMenu.count() == 0) return;

        for(int i=0; i<_markerMenu.count(); i++)
            menu.addAction( _markerMenu[i].text );
    }else
    {
        if(_mapMenu.count() == 0) return;

        for(int i=0; i<_mapMenu.count(); i++)
            menu.addAction( _mapMenu[i].text );
    }


    QAction *selectedAction  = menu.exec(gpos);
    if(selectedAction)
    {
        bool found =false;
        for(int i=0; i<_markerMenu.count();i++)
        {
            if(_markerMenu[i].text == selectedAction->text())
            {
                mitem.text = _markerMenu[i].text;
                mitem.id = _markerMenu[i].id;
                emit menuAction( mitem );
                found = true;
                break;
            }
        }

        if(!found)
        {
            for(int i=0; i<_mapMenu.count();i++)
            {
                if(_mapMenu[i].text == selectedAction->text())
                {
                    mitem.text = _mapMenu[i].text;
                    mitem.id = _mapMenu[i].id;
                    emit menuAction( mitem );
                    break;
                }
            }
        }

        selectedAction->deleteLater();
    }
}

void OSMWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(_selectedMarker != nullptr && _selectedRoute != nullptr)
        {
            if(_selectedRoute->editable)
                _editidMarker = _selectedMarker;
        }
        else
        {
            _lastMousePos = e->localPos();
            _drag = true & _canMouseDrag;
        }
    }
}

void OSMWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        _editidMarker = nullptr;
        _drag = false;
    }
}

void OSMWidget::mouseMoveEvent(QMouseEvent *e)
{
    QPointF mousePoint = e->localPos();

    if(_drag)
    {
        QPointF dmouse = _lastMousePos - mousePoint;
        _translate -= dmouse * _dragSpeed;
        emit mapMoved( getGPSPossition(widgetCenter()) );
    }else
    {
        if(_editidMarker != nullptr)
        {
            _selectedMarker = nullptr;
            _selectedRoute = nullptr;
            _editidMarker->possition = getGPSPossition( e->localPos() );
        }
        else
        {
            updateSelection(mousePoint.toPoint());
        }
    }

    repaint();

    _lastMousePos = mousePoint;
    emit mouseMoved( getGPSPossition( mousePoint.toPoint() ) );
}

void OSMWidget::mouseDoubleClickEvent(QMouseEvent * e)
{
    if(e->button() == Qt::LeftButton)
    {
        if(_selectedMarker != nullptr)
        {
            _editidMarker = _selectedMarker;
        }
    }
}

void OSMWidget::wheelEvent(QWheelEvent *e)
{
    if(_canMouseZoom)
    {
        int new_zoom = _zoom + (e->delta() > 0 ? 1 : -1);
        if(new_zoom < _minZoom) new_zoom = _minZoom;
        if(new_zoom > _maxZoom) new_zoom = _maxZoom;

        if(new_zoom != _zoom)
        {
            setGPSAndZoom(getGPSPossition(_lastMousePos.toPoint()),new_zoom,_lastMousePos.toPoint());
        }
    }

}

void OSMWidget::paintEvent(QPaintEvent *)
{
    updateMapTiles();

    QPainter painter;
    painter.begin(this);
    painter.translate(_translate);

    renderMapTiles(painter);
    renderRoutes(painter);
}


void OSMWidget::updateMapTiles()
{
    tryAddMissingDirectories();

    for(int i = _tiles.size()-1; i >= 0; i--)
        if(!isTileVisible( _tiles[i] ))
            _tiles.removeAt(i);

    QPointF tile_size = GetTileSize();
    int wx = width() / tile_size.x();
    int wy = height() / tile_size.y();

    int tx = int(_translate.x() / tile_size.x());
    int ty = int(_translate.y() / tile_size.y());

    for(int i = 0; i <_tiles.size(); i++ )
    {
        if(_tiles[i].pixmap.isNull())
            updateTile(&_tiles[i]);
    }

    for(int ix = -1; ix<=wx + 1; ix++)
    {
        for(int iy = -1; iy<=wy + 1; iy++)
        {
            bool tile_already_exists = false;
            for(int i = 0; i <_tiles.size(); i++ )
            {
                if(_tiles[i].ix == ix - tx &&
                        _tiles[i].iy == iy - ty)
                {
                    tile_already_exists = true;
                    break;
                }
            }

            if(!tile_already_exists)
                addTile(ix - tx, iy - ty,_zoom);
        }
    }
}

void OSMWidget::renderMapTiles(QPainter &painter)
{
    QPointF tile_size = GetTileSize();
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(int i=0;i<_tiles.count(); i++)
    {
        if(_tiles[i].pixmap.isNull())
        {
            painter.drawPixmap( _tiles[i].ix * tile_size.x(), _tiles[i].iy * tile_size.y(), tile_size.x(), tile_size.y(), _errorPixMap);
        }
        else
        {
            painter.drawPixmap( _tiles[i].ix * tile_size.x(), _tiles[i].iy * tile_size.y(), tile_size.x(), tile_size.y(), _tiles[i].pixmap);
        }
    }
}

void OSMWidget::renderRoutes(QPainter &painter)
{
    QPen last_pen = painter.pen();
    QBrush last_brush = painter.brush();

    for(int i=0;i<_routes.count();i++)
    {
        if(_routes[i].visible)
            renderRoute(painter, _routes[i]);
    }

    painter.setPen( last_pen );
    painter.setBrush( last_brush );
    painter.setOpacity( 1.0 );
}

void OSMWidget::renderRoute(QPainter &painter, OSMRoute &route)
{
    if(route.route.count() == 0) return;
    if(route.editable)
        painter.setOpacity(route.enabledOpacity);
    else
        painter.setOpacity(route.disabledOpacity);

    route.route[0].type = route.stdFirstMarkerType;
    if(route.route.count() > 2 && route.closed)
    {
        //complitePen
        OSMMarker *m1 = &route.route[0];
        OSMMarker *m2 = &route.route[route.route.count() - 1];

        QPointF mpos1;
        mpos1.setX( long2tilex( m1->possition.x(), _zoom )*_tile_size.x() );
        mpos1.setY( lat2tiley( m1->possition.y(), _zoom )*_tile_size.y() );

        QPointF mpos2;
        mpos2.setX( long2tilex( m2->possition.x(), _zoom )*_tile_size.x() );
        mpos2.setY( lat2tiley( m2->possition.y(), _zoom )*_tile_size.y() );

        painter.setPen( route.complitePen );
        painter.drawLine(mpos1, mpos2);
    }

    for(int i=0;i<route.route.count() - 1; i++)
    {
        OSMMarker *m1 = &route.route[i];
        OSMMarker *m2 = &route.route[i + 1];
        painter.setPen( route.pen );

        renderRouteSegment(painter, m1,m2);
    }

    OSMMarker *mlast = &route.route[route.route.count() - 1];
    renderMarker(painter, mlast);

}

void OSMWidget::renderRouteSegment(QPainter &painter, OSMMarker *m1, OSMMarker *m2)
{
    if(!isMarkerVisible(m1) && !isMarkerVisible(m2) )
        return;

    QPointF mpos1;
    mpos1.setX( long2tilex( m1->possition.x(), _zoom )*_tile_size.x() );
    mpos1.setY( lat2tiley( m1->possition.y(), _zoom )*_tile_size.y() );

    QPointF mpos2;
    mpos2.setX( long2tilex( m2->possition.x(), _zoom )*_tile_size.x() );
    mpos2.setY( lat2tiley( m2->possition.y(), _zoom )*_tile_size.y() );

    painter.drawLine( mpos1, mpos2 );

    renderMarker(painter, m1);
}

void OSMWidget::renderMarker(QPainter &painter, OSMMarker *m)
{
    if(!isMarkerVisible(m)) return;

    QPen last_pen = painter.pen();
    QBrush last_brush = painter.brush();

    QPointF mpos = getPointFromGPS(m->possition);

    painter.save();

    painter.translate(mpos.x(), mpos.y());
    painter.rotate(m->rotate);

    qreal marker_size = m->size;
    if(m == _selectedMarker)
    {
        painter.setPen( m->penWhenSelected );
        painter.setBrush( m->bgBrushWhenSelected );
        marker_size = m->sizeWhenSelected;
    }else
    {
        painter.setPen( m->pen );
        painter.setBrush( m->bgBrush );
    }

    if(m->type == OSMMarkerType::Circle)
    {
        painter.drawEllipse(mpos, marker_size, marker_size );
    }
    else if(m->type == OSMMarkerType::Square)
    {
        painter.drawRect(mpos.x() - marker_size/2.0,
                         mpos.y() - marker_size/2.0,
                         marker_size,    marker_size);
    }else if(m->type == OSMMarkerType::Triangle
             || m->type == OSMMarkerType::Triangle2
             || m->type == OSMMarkerType::Triangle3)
    {
        qreal q23 = (2.0/3.0);
        qreal h23 = marker_size*q23;
        qreal sin60 = sin( 1.570796327 * q23 );
        qreal cos60 = cos( 1.570796327 * q23 );

        QPolygon pol;

        if(m->type == OSMMarkerType::Triangle)
        {
            pol.append(QPointF(mpos.x(), mpos.y() - h23).toPoint());
            pol.append(QPointF(mpos.x() + h23*sin60, mpos.y() + h23*cos60).toPoint());
            pol.append(QPointF(mpos.x() - h23*sin60, mpos.y() + h23*cos60).toPoint());
        }else if(m->type == OSMMarkerType::Triangle2)
        {
            pol.append(QPointF(mpos.x(), mpos.y() + h23).toPoint());
            pol.append(QPointF(mpos.x() + h23*sin60, mpos.y() - h23*cos60).toPoint());
            pol.append(QPointF(mpos.x() - h23*sin60, mpos.y() - h23*cos60).toPoint());
        }else if(m->type == OSMMarkerType::Triangle3)
        {
            pol.append(QPointF(0, -h23).toPoint());
            pol.append(QPointF(h23*sin60/2.0,h23*cos60).toPoint());
            pol.append(QPointF(-h23*sin60/2.0, h23*cos60).toPoint());
        }

        painter.drawPolygon(pol);
    }
    /*
     if(m->type == OSMMarkerType::Circle)
    {
        painter.drawEllipse(mpos, marker_size, marker_size );
    }
    else if(m->type == OSMMarkerType::Square)
    {
        painter.drawRect(mpos.x() - marker_size/2.0,
                         mpos.y() - marker_size/2.0,
                         marker_size,    marker_size);
    }else if(m->type == OSMMarkerType::Triangle
             || m->type == OSMMarkerType::Triangle2
             || m->type == OSMMarkerType::Triangle3)
    {
        qreal q23 = (2.0/3.0);
        qreal h23 = marker_size*q23;
        qreal sin60 = sin( 1.570796327 * q23 );
        qreal cos60 = cos( 1.570796327 * q23 );

        QPolygon pol;

        if(m->type == OSMMarkerType::Triangle)
        {
            pol.append(QPointF(mpos.x(), mpos.y() - h23).toPoint());
            pol.append(QPointF(mpos.x() + h23*sin60, mpos.y() + h23*cos60).toPoint());
            pol.append(QPointF(mpos.x() - h23*sin60, mpos.y() + h23*cos60).toPoint());
        }else if(m->type == OSMMarkerType::Triangle2)
        {
            pol.append(QPointF(mpos.x(), mpos.y() + h23).toPoint());
            pol.append(QPointF(mpos.x() + h23*sin60, mpos.y() - h23*cos60).toPoint());
            pol.append(QPointF(mpos.x() - h23*sin60, mpos.y() - h23*cos60).toPoint());
        }else if(m->type == OSMMarkerType::Triangle3)
        {
            pol.append(QPointF(mpos.x(), mpos.y() - h23).toPoint());
            pol.append(QPointF(mpos.x() + h23*sin60/2.0, mpos.y() + h23*cos60).toPoint());
            pol.append(QPointF(mpos.x() - h23*sin60/2.0, mpos.y() + h23*cos60).toPoint());
        }

        painter.drawPolygon(pol);
    }
    */

    painter.restore();
    painter.setPen( last_pen );
    painter.setBrush( last_brush );
}

void OSMWidget::addTile(int ix, int iy, int iz)
{
    QPixmap pixmap(getCachedTileFilePath(ix, iy, iz));
    if(pixmap.isNull())
    {
        downlaodTile(ix,iy,iz);
    }

    OSMTile tile;
    tile.ix = ix;
    tile.iy = iy;
    tile.pixmap = pixmap;
    _tiles.append(tile);
}

void OSMWidget::setResources(QString osmUrl, QString cacheDir, QString format, QString errorTile)
{
    _osmUrl = osmUrl;
    _cacheDir = cacheDir;
    _tileFormat = format;
    _errorTileFilePath = errorTile;

    if(_osmUrl.endsWith("/"))
        _osmUrl = _osmUrl.left(_osmUrl.length() - 1);

    if(!_cacheDir.endsWith("/"))
        _cacheDir.append("/");

    _cacheSubDir = osmUrl;
    _cacheSubDir.replace("/","_").replace(':',"");

    if(_cacheSubDir.endsWith("_"))
        _cacheSubDir = _cacheSubDir.left(_cacheSubDir.length() - 1);

    _cacheSubDir.append("/");

    _errorPixMap = QPixmap(getErrorTileFilePath());
}

QString OSMWidget::getTileUrl(int ix, int iy, int iz) const
{
    return QString("%1/%2/%3/%4.%5")
            .arg(_osmUrl)
            .arg(iz).arg(ix).arg(iy)
            .arg(_tileFormat);
}

QString OSMWidget::getCachedTileFileName(int ix, int iy, int iz) const
{
    return QString("tile_%1_%2_%3.%4")
            .arg(iz).arg(ix).arg(iy)
            .arg(_tileFormat);
}

QString OSMWidget::getCachedTileFilePath(int ix, int iy, int iz) const
{
    return _cacheDir + _cacheSubDir + getCachedTileFileName(ix, iy, iz);
}


QString OSMWidget::getCachedTileFileDir() const
{
    return _cacheDir + _cacheSubDir.left(_cacheSubDir.length() - 1);
}

QString OSMWidget::getErrorTileFilePath() const
{
    return _errorTileFilePath;
}

bool OSMWidget::tileFileExists(int ix, int iy, int iz)
{
    return QFile(getCachedTileFilePath(ix, iy, iz)).exists();
}

QString OSMWidget::getCacheDir() const
{
    return _cacheDir;
}

QString OSMWidget::getCacheSubDir() const
{
    return _cacheSubDir;
}

void OSMWidget::copyErrorTileAsNewTile(int ix, int iy, int iz)
{
    tryAddMissingDirectories();
    QFile::copy(getErrorTileFilePath(),getCachedTileFilePath(ix, iy, iz));
}

void OSMWidget::downlaodTile(int ix, int iy, int iz)
{
    QString url = getTileUrl(ix, iy, iz);
    if(_getRequestsQueue.contains(url)) return;
    _getRequestsQueue.append( url );
    _downlaodTile();
}

void OSMWidget::_downlaodTile()
{
    if(_waitingForRequestResp != "") return;
    if(_getRequestsQueue.count() == 0 ) return;
    _waitingForRequestResp = _getRequestsQueue[0];
    _getRequestsQueue.removeFirst();

    QString url = _waitingForRequestResp;
    if(_getRequestsQueue.contains(url)) return;
    qDebug() << "downlading: " << url;

    QNetworkRequest request;
    request.setUrl( url );

    QNetworkReply * reply = _accessManager->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(onTileDownloaded()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(onTileDownloadingError(QNetworkReply::NetworkError)));
}

bool OSMWidget::isTileVisible(OSMTile &tile)
{
    QPointF tile_size = GetTileSize();

    double x = tile.ix;
    double y = tile.iy;
    if(x*tile_size.x() + ( _translate.x() ) < -tile_size.x()) return false;
    if(x*tile_size.x() + ( _translate.x() ) > width()) return false;
    if(y*tile_size.y() + ( _translate.y() ) < -tile_size.y()) return false;
    if(y*tile_size.y() + ( _translate.y() ) > height()) return false;

    return true;
}

void OSMWidget::updateTile(OSMTile *tile)
{
    QString path = getCachedTileFilePath(tile->ix, tile->iy, _zoom);
    qDebug() << path;
    QPixmap pixmap(path);
    if(pixmap.isNull())
        downlaodTile(tile->ix,tile->iy,_zoom);

    tile->pixmap = pixmap;
}

void OSMWidget::loadErrorPixMap()
{
    _errorPixMap.load(getErrorTileFilePath());
    if(_errorPixMap.isNull())
    {
        _errorPixMap = QPixmap(256,256);
        _errorPixMap.fill(Qt::black);
    }
}

QPointF OSMWidget::GetTileSize()
{
    return _tile_size;
}

void OSMWidget::updateSelection(QPoint mouse)
{
    //QPointF p = getGPSPossition( mouse );

    qreal ndist = -1.0;

    OSMMarker * nmarker = nullptr;
    OSMRoute * nroute = nullptr;

    for(int r=0; r<_routes.count(); r++)
    {
       for(int m = 0; m<_routes[r].route.count(); m++)
       {
           if(!_routes[r].visible) continue;
           OSMMarker * marker = &_routes[r].route[m];
           QPointF mpos = getPointFromGPS( marker->possition ) + _translate;

           qreal dist = sqrt( pow(mpos.x() - mouse.x(),2) + pow(mpos.y() - mouse.y(),2) );
           if(dist <= marker->size && (dist < ndist || nmarker == nullptr))
           {
               ndist = dist;
               nmarker = marker;
               nroute = &_routes[r];
           }
       }
    }

    bool emitSignal = false;
    if(_selectedMarker == nullptr && nmarker != nullptr)
        emitSignal = true;

    _selectedMarker = nmarker;
    _selectedRoute = nroute;

    if(emitSignal)
        emit markerSelected(_selectedMarker);
}

bool OSMWidget::markerSelectEnable() const
{
    return _markerSelectEnable;
}

void OSMWidget::setMarkerSelectEnable(bool markerSelectEnable)
{
    _markerSelectEnable = markerSelectEnable;
}


bool OSMWidget::isMarkerVisible(OSMMarker *m)
{
    QPointF mpos;
    mpos.setX( long2tilex( m->possition.x(), _zoom )*_tile_size.x() );
    mpos.setY( lat2tiley( m->possition.y(), _zoom )*_tile_size.y() );

    if(mpos.x() + _translate.x() < 0 ) return false;
    if(mpos.y() + _translate.y() < 0 ) return false;

    if(mpos.x() + _translate.x() > width() ) return false;
    if(mpos.y() + _translate.y() > height() ) return false;

    return true;
}

void OSMWidget::addMapMenu(qint32 id, QString text)
{
    OSMMenuItem item;
    item.id = id;
    item.text = text;
    _mapMenu.append( item );
}

void OSMWidget::addMarkerMenu(qint32 id, QString text)
{
    OSMMenuItem item;
    item.id = id;
    item.text = text;
    _markerMenu.append( item );
}


void OSMWidget::setMarkerEdit(OSMMarker *marker)
{
    _editidMarker = marker;
}

void OSMWidget::removeRoute(int index)
{
    _routes.removeAt( index );
}

void OSMWidget::removeRoute(QString name)
{
    for(int i=0;i<_routes.count();i++)
    {
        if(_routes[i].name == name)
        {
            removeRoute(i);
            return;
        }
    }
}

void OSMWidget::removeAllRoutes()
{
    _routes.clear();
}

bool OSMWidget::canMouseZoom() const
{
    return _canMouseZoom;
}

void OSMWidget::setCanMouseZoom(bool canMouseZoom)
{
    _canMouseZoom = canMouseZoom;
}

bool OSMWidget::canMouseDrag() const
{
    return _canMouseDrag;
}

void OSMWidget::setCanMouseDrag(bool canMouseDrag)
{
    _canMouseDrag = canMouseDrag;
}


void OSMWidget::setZoom(int newZoom)
{
    QPointF gpsPos = getGPSPossition(QPoint(width()/2, height()/2));
    _zoom = newZoom;
    setGPSPossition( gpsPos , QPoint(width()/2, height()/2));
    _tiles.clear();

    emit zoomChanged(_zoom);

    repaint();
}

qint32 OSMWidget::toDownload()
{
    return _getRequestsQueue.count();
}

void OSMWidget::setGPSPossition(QPointF p, QPointF sp)
{
    QPointF tile_size = GetTileSize();
    double tx = long2tilex(p.x(), _zoom) * tile_size.x();
    double ty = lat2tiley(p.y(), _zoom) * tile_size.y();

    _translate = QPointF( -tx + sp.x(), -ty + sp.y());

    emit mapMoved( getGPSPossition(widgetCenter()) );

    _getRequestsQueue.clear();
    repaint();
}

QPointF OSMWidget::getGPSPossition(QPointF sp)
{
    QPointF tile_size = GetTileSize();
    return QPointF(
        tilex2long( (-_translate.x() + sp.x())  / tile_size.x(), _zoom),
        tiley2lat( (-_translate.y() + sp.y()) / tile_size.y(), _zoom));
}

void OSMWidget::setGPSAndZoom(QPointF gpsPos, int newZoom, QPointF sp)
{
    _zoom = newZoom;
    setGPSPossition( gpsPos , sp);
    _tiles.clear();

    emit zoomChanged(_zoom);

    _getRequestsQueue.clear();
    repaint();
}

QPointF OSMWidget::getPointFromGPS(QPointF gps)
{
    QPointF mpos;
    mpos.setX( long2tilex( gps.x(), _zoom )*_tile_size.x() );
    mpos.setY( lat2tiley( gps.y(), _zoom )*_tile_size.y() );

    return mpos;
}

void OSMWidget::setTileSize(double size)
{
    _tile_size =  QPointF(STD_TILE_WIDTH * size, STD_TILE_HEIGHT * size);
}

void OSMWidget::addRoute(OSMRoute route)
{
    if(getRoute(route.name) != nullptr) return;
    _routes.append(route);
    repaint();
}

void OSMWidget::centreGPSPossition(QPointF p)
{
    setGPSPossition(p, widgetCenter());
}

OSMRoute *OSMWidget::getRoute(int index)
{
    if(index <0) return nullptr;
    if(index >= _routes.count()) return nullptr;

    return &_routes[index];
}

OSMRoute *OSMWidget::getRoute(QString name)
{
    for(int i=0;i<_routes.count();i++)
        if(_routes[i].name == name)
            return &_routes[i];

    return nullptr;
}

int OSMWidget::getRouteCount()
{
    return _routes.count();
}

QPoint OSMWidget::widgetCenter()
{
    return QPoint(width()/2.0,height()/2.0);
}

void OSMWidget::setZoomRange(int minZoom, int maxZoom)
{
    _minZoom = minZoom;
    _maxZoom = maxZoom;
}

void OSMWidget::tryAddMissingDirectories()
{
    QDir cacheDir(getCachedTileFileDir());
    if(!cacheDir.exists())
    {
        cacheDir = QDir(getCacheDir());
        if(!cacheDir.exists())
        {
            qDebug() << "Cache directory dont exits: " << getCacheDir();
        }else
        {
            if(!cacheDir.mkdir( getCacheSubDir() ))
                qDebug() << "Can't create subdirectory for cache: " << getCachedTileFileDir();
        }
    }
}


OSMMarker::OSMMarker()
{
    static int _name_num = 0;
    name = QString("marker_%1").arg( _name_num++ );

    QColor color = QColor(Qt::black);
    QBrush brush = QBrush(color, Qt::SolidPattern);

    pen = QPen(brush,2);
    size = 20.0;
    sizeWhenSelected = 25.0;
    type = OSMMarkerType::Triangle;

    QColor bcolor = AppSettings::GetKey( AppSettings::MapEdit_Marker_Color ).value<QColor>();
    bgBrush = QBrush(bcolor);

    penWhenSelected = QPen(brush,3);

    QColor ycolor = AppSettings::GetKey( AppSettings::MapEdit_Active_Marker_Color ).value<QColor>();
    bgBrushWhenSelected = QBrush(ycolor);

    rotate = 0;
}


OSMRoute::OSMRoute()
{
    static int _name_num = 0;
    name = QString("route_%1").arg(_name_num);
    QColor color = QColor(Qt::black);
    QBrush brush = QBrush(color, Qt::SolidPattern);
    pen = QPen(brush,2);

    _marker_counter = 0;

    QColor bcolor = AppSettings::GetKey( AppSettings::MapEdit_Marker_Color ).value<QColor>();
    stdBgBrush  = QBrush(bcolor);
    stdMarkerPen = QPen(brush,2);
    stdMarkerSize = 20;
    stdMarkerType = OSMMarkerType::Triangle2;
    stdFirstMarkerType = OSMMarkerType::Square;
    closed = false;
    visible = true;
    editable = true;
    enabledOpacity = AppSettings::GetKey( AppSettings::MapEdit_Enabled_Route_Opacity ).toDouble();
    disabledOpacity = AppSettings::GetKey( AppSettings::MapEdit_Disabled_Route_Opacity ).toDouble();

    QColor compliteColor = QColor(Qt::magenta);
    QBrush compliteBrush =  QBrush(compliteColor, Qt::SolidPattern);
    complitePen = QPen(compliteBrush,2, Qt::DashDotLine);
}

int OSMRoute::addPoint(QPointF pos)
{
    OSMMarker mark;

    mark.name = name + QString("_%1").arg(_marker_counter++);
    mark.bgBrush = stdBgBrush;
    mark.pen = stdMarkerPen;
    mark.possition = pos;
    mark.size = stdMarkerSize;
    mark.type = stdMarkerType;

    route.append(mark);

    return route.count() - 1;
}

int OSMRoute::addPoint(QPointF pos, int before)
{
    OSMMarker mark;

    mark.name = name + QString("_%1").arg(_marker_counter++);
    mark.bgBrush = stdBgBrush;
    mark.pen = stdMarkerPen;
    mark.possition = pos;
    mark.size = stdMarkerSize;
    mark.type = stdMarkerType;

    route.insert(before, mark);

    return route.count() - 1;
}

OSMMarker *OSMRoute::getMarker(QString name)
{
    for(int i=0;i<route.count();i++)
        if(route[i].name == name)
            return &route[i];

    return nullptr;
}

OSMMarker *OSMRoute::getMarker(int index)
{
    if( index >= route.count()) return nullptr;
    return &route[index];
}
