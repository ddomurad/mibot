#ifndef TRACKPROVIDER_H
#define TRACKPROVIDER_H

#include <QtCore>
#include "Forms/OSMWidget.h"

class TrackProvider
{
public:
    ~TrackProvider();

    static QStringList GetTracks();
    static void LoadRoutesFromFile();
    static void SelectRoute(QString);

    static OSMRoute *SelectedRoute();
    static OSMMarker *ActivePoint();

    static OSMMarker *FirstPoint();
    static OSMMarker *NextPoint();
    static OSMMarker *PrevPoint();

    static int ActivePointId();
    static void SetActivePoint(int);
private:
    TrackProvider();

    static QList<OSMRoute> routes;
    static void removeAllRoutes();

    static int _selected_route;
    static int _active_point;
};

#endif // TRACKPROVIDER_H
