#ifndef OSMWIDGET_H
#define OSMWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QtNetwork>
#include <QPen>

#define STD_TILE_WIDTH 256.0
#define STD_TILE_HEIGHT 256.0

class OSMTile
{
public:
    int ix;
    int iy;
    QPixmap pixmap;
};

enum class OSMMarkerType { Circle, Square, Triangle, Triangle2, Triangle3};

class OSMRoute;

class OSMMarker
{
public:
    OSMMarker();

    QString name;
    QPointF possition;
    QPen    pen;
    QBrush  bgBrush;
    QPen    penWhenSelected;
    QBrush  bgBrushWhenSelected;
    qreal   size;
    qreal   sizeWhenSelected;
    qreal   rotate;
    OSMMarkerType type;    
};

class OSMRoute
{
public:
    OSMRoute();

    QString name;
    QPen pen;
    QPen    complitePen;
    QList<OSMMarker> route;

    qreal stdMarkerSize;
    OSMMarkerType stdMarkerType;
    OSMMarkerType stdFirstMarkerType;
    QPen stdMarkerPen;
    QBrush  stdBgBrush;

    int addPoint(QPointF pos);
    int addPoint(QPointF pos, int before);

    OSMMarker * getMarker(QString name);
    OSMMarker * getMarker(int index);

    bool        closed;
    bool        visible;
    bool        editable;
    double      disabledOpacity;
    double      enabledOpacity;
private:
    int _marker_counter;
};

class OSMMenuItem
{
public:
    qint32      id;
    QString     text;

    OSMRoute    *route;
    OSMMarker   *marker;
    QPoint       mouePos;
    QPointF      gpsPos;
};

class OSMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OSMWidget(QWidget *parent = 0);
    ~OSMWidget();

    qint32 toDownload();

    void setGPSPossition(QPointF p, QPointF sp);
    QPointF getGPSPossition(QPointF sp);

    void setZoomRange(int minZoom, int maxZoom);
    void setResources(QString osmUrl, QString getCacheDir, QString format, QString errorTile);

    void setGPSAndZoom(QPointF gpsPos,int newZoom, QPointF sp);

    QPointF getPointFromGPS(QPointF gps);

    OSMRoute *getRoute(int index);
    OSMRoute *getRoute(QString name);
    QList<OSMRoute> *getRoutes();
    int       getRouteCount();

    QPoint widgetCenter();

    bool canMouseDrag() const;
    void setCanMouseDrag(bool canMouseDrag);
    bool canMouseZoom() const;
    void setCanMouseZoom(bool canMouseZoom);
    bool enableDbClick() const;
    void setEnableDbClick(bool enableDbClick);
    bool markerSelectEnable() const;
    void setMarkerSelectEnable(bool markerSelectEnable);

    void addMapMenu(qint32 id, QString text);
    void addMarkerMenu(qint32 id, QString text);

    void setMarkerEdit(OSMMarker * marker);
    void setTargetMarker(OSMMarker * marker);


    void removeRoute(int index);
    void removeRoute(QString name);
    void removeAllRoutes();

signals:
    void mapMoved(QPointF gpsPossitionOnWidgetCenter);
    void zoomChanged(int zoom);
    void tileDownloadingDone(int left);
    void mouseMoved(QPointF gpsPos);
    void menuAction(OSMMenuItem item);
    void markerSelected(OSMMarker * markerSelected);

public slots:
    void setZoom(int newZoom);
    void setTileSize(double size);
    void addRoute(OSMRoute route);
    void centreGPSPossition(QPointF p);

private slots:
    void onTileDownloaded();
    void onTileDownloadingError(QNetworkReply::NetworkError e);
    void onDownloadTimer();
    void onContextMenu(const QPoint &pos);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    void paintEvent(QPaintEvent *);

    void updateMapTiles();
    void renderMapTiles(QPainter &painter);
    void renderMarkers(QPainter &painter);
    void renderMarker(QPainter &painter, OSMMarker *marker);
    void renderRoutes(QPainter &painter);
    void renderRoute(QPainter &painter, OSMRoute & route);
    void renderRouteSegment(QPainter &painter, OSMMarker *m1, OSMMarker *m2);

    void addTile(int ix, int iy, int iz);

    QString getTileUrl(int ix, int iy, int iz) const;
    QString getCachedTileFileName(int ix, int iy, int iz) const;
    QString getCachedTileFilePath(int ix, int iy, int iz) const;
    QString getCachedTileFileDir() const;
    QString getErrorTileFilePath() const;
    bool    tileFileExists(int ix, int iy, int iz);

    QString getCacheDir() const;
    QString getCacheSubDir() const;

    void tryAddMissingDirectories();
    void copyErrorTileAsNewTile(int ix, int iy, int iz);

    void downlaodTile(int ix, int iy, int iz);
    void _downlaodTile();

    bool isTileVisible(OSMTile &tile);
    bool isMarkerVisible(OSMMarker *m);

    void updateTile(OSMTile *tile);
    void loadErrorPixMap();

    QPointF GetTileSize();

    void updateSelection(QPoint mouse);
private:

    bool _canMouseDrag;
    bool _canMouseZoom;
    bool _markerSelectEnable;

    bool _drag;
    qreal _dragSpeed;
    QPointF _translate;
    QPointF _lastMousePos;

    qint8 _zoom;
    qint8 _minZoom;
    qint8 _maxZoom;

    QList<OSMTile> _tiles;

    QString _osmUrl;
    QString _cacheDir;
    QString _cacheSubDir;
    QString _tileFormat;
    QString _errorTileFilePath;

    QNetworkAccessManager * _accessManager;

    QList<QString> _getRequestsQueue;
    QString        _waitingForRequestResp;
    QPixmap _errorPixMap;
    QTimer *_download_timer;

    QPointF _tile_size;
    QList<OSMRoute> _routes;

    QList<OSMMenuItem> _mapMenu;
    QList<OSMMenuItem> _markerMenu;

    OSMMarker * _selectedMarker;
    OSMMarker * _targetMarker;
    OSMRoute * _selectedRoute;


    OSMMarker  * _editidMarker;

};

#endif // OSMWIDGET_H
