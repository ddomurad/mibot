#ifndef OSMWIDGET_H
#define OSMWIDGET_H

#include <QDir>
#include <QFile>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class OSMTileDesc;

class OSMResourceDesc
{
public:
    OSMResourceDesc();
    OSMResourceDesc(QString osmUrl, QString cacheDir, QString format, QString errorTile);
    ~OSMResourceDesc();

    QString getTileUrl(OSMTileDesc &) const;
    QString getCachedTileFileName(OSMTileDesc &) const;
    QString getCachedTileFilePath(OSMTileDesc &) const;
    QString getCachedTileFileDir() const;
    QString getErrorTileFilePath() const;
    bool    tileFileExists(OSMTileDesc &);

    QString cacheDir() const;
    QString cacheSubDir() const;

    void Setup(OSMResourceDesc &);
private:
    QString _osmUrl;
    QString _cacheDir;
    QString _cacheSubDir;
    QString _tileFormat;
    QString _errorTileFilePath;
};

class OSMTileDesc
{
public:
    OSMTileDesc();
    OSMTileDesc(int ix, int iy, int iz);

    int ix;
    int iy;
    int iz;
};

class OSMTileItem : public QGraphicsItem
{
public:
    OSMTileItem(OSMTileDesc & , OSMResourceDesc *, QGraphicsItem *parent = nullptr);
    ~OSMTileItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void updateTile(bool force);

    bool isValid();
    QPixmap pixmap();
    OSMTileDesc tileDesc();

private:
    bool _isValid;
    QPixmap _pixmap;
    OSMResourceDesc * _resDesc;
    OSMTileDesc _tileDesc;
};

class OSMScene : public QGraphicsScene
{
    Q_OBJECT
public:
    OSMScene(OSMResourceDesc *, QObject * parent);

public slots:
    void updateTile(OSMTileDesc t);

    OSMTileItem * getTileItem(OSMTileDesc &t);
    void addTile(OSMTileDesc &t);
    void updateScene();
    void setZoom(int z);
    void focusGPSPoint(QPointF p);
    void enableScrollZoom(bool enable);

signals:
    void downlaodTile(OSMTileDesc trd);

private:
    OSMResourceDesc * _resDesc;
    int _iz;
    bool _enableScrollZoom;
    bool _drag;
    QPointF _translation;
    QGraphicsRectItem * _secret_item;
    // QGraphicsScene interface
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class OSMTileDownloader : public QObject
{
    Q_OBJECT
public:
    OSMTileDownloader(OSMResourceDesc *, QObject * parent);
    ~OSMTileDownloader();

    bool isWaitingForTile(OSMTileDesc & trd);
    int waitingRequests();

signals:
    void tileDownloaded(OSMTileDesc);
    void allTilesDownloaded();

private slots:

    void downlaodTile(OSMTileDesc trd);
    void onTileDownloaded();
    void onTileDownloadingError(QNetworkReply::NetworkError);

private:

    bool getAndRemoveFromListCorrespondingGetRequest(QNetworkRequest &, OSMTileDesc * getReqInfo);
    void copyErrorTileAsNewTile(OSMTileDesc & trd);
    void tryAddMissingDirectories();

    QNetworkAccessManager * _accessManager;
    QList<OSMTileDesc> _getsWaiting;
    OSMResourceDesc * _resDesc;
};

class OSMWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OSMWidget(QWidget *parent = 0);
    ~OSMWidget();

    void setResourcesDesc(OSMResourceDesc desc);
    void setZoom(int z);
    void focusPoint(QPointF p);
    void enableScrollZoom(bool enable);

signals:
    void pedingTilesCountChanged(int);

public slots:

private slots:
    void onTileDownloaded(OSMTileDesc);
    void onAllTilesDownladed();


private:
    QGraphicsView * _graphicsView;
    OSMScene * _scene;

    OSMResourceDesc *_resourceDesc;
    OSMTileDownloader * _tileDownloader;
};

#endif // OSMWIDGET_H
