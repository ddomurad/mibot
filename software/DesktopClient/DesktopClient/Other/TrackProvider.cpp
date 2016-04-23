#include "TrackProvider.h"

TrackProvider::TrackProvider()
{
    _selected_route = -1;
    _active_point = 0;
}

void TrackProvider::removeAllRoutes()
{
    routes.clear();
}

TrackProvider::~TrackProvider()
{
    removeAllRoutes();
}

QStringList TrackProvider::GetTracks()
{
    QStringList list;
    for(int i=0;i<routes.count();i++)
        list.append(routes[i].name);

    return list;
}

void TrackProvider::LoadRoutesFromFile()
{
    removeAllRoutes();

    QFile file("./routes");
    if(!file.open(QIODevice::ReadOnly))
        return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject routesObj = doc.object();

    file.close();

    if(!routesObj["routes"].isArray()) return;
    QJsonArray routesArray = routesObj["routes"].toArray();

    for(int i=0;i<routesArray.count(); i++)
    {
        QJsonObject routeObj = routesArray.at(i).toObject();
        bool closed = routeObj["closed"].toBool();
        bool visible = routeObj["visible"].toBool();
        bool editable = routeObj["editable"].toBool();
        QString name = routeObj["name"].toString();

        OSMRoute newRoute;
        newRoute.name = name;
        newRoute.closed = closed;
        newRoute.visible = visible;
        newRoute.editable = editable;

        QJsonArray routePoints = routeObj["points"].toArray();
        for(int j=0; j<routePoints.count(); j++)
        {
            QJsonObject pointObj = routePoints.at(j).toObject();
            QPointF point;
            point.setX( pointObj["pos_x"].toDouble());
            point.setY( pointObj["pos_y"].toDouble());

            newRoute.addPoint(point);
        }

        routes.append(newRoute);
    }
}

void TrackProvider::SelectRoute(QString name)
{
    _active_point = 0;
    for(int i=0;i<routes.count();i++)
        if(routes[i].name == name)
        {
            _selected_route = i;
            return;
        }

    _selected_route = -1;
}

OSMRoute *TrackProvider::SelectedRoute()
{
    if(_selected_route == -1)
    return nullptr;

    return &routes[_selected_route];
}

OSMMarker *TrackProvider::ActivePoint()
{
    if(_selected_route == -1 )
        return nullptr;

    if(_active_point == -1 )
        return nullptr;

    if(SelectedRoute()->route.count() == 0)
        return nullptr;

    if(_active_point >= SelectedRoute()->route.count())
        return nullptr;

    return &SelectedRoute()->route[_active_point];
}

OSMMarker *TrackProvider::FirstPoint()
{
    _active_point = 0;
    return ActivePoint();
}

OSMMarker *TrackProvider::NextPoint()
{
    _active_point++;
    OSMRoute * sr = SelectedRoute();
    if(sr == nullptr) return nullptr;
    if(_active_point >= sr->route.count())
    {
        if(sr->closed)
            _active_point = 0;
        else
            _active_point = -1;
    }

    return ActivePoint();
}

OSMMarker *TrackProvider::PrevPoint()
{
    _active_point--;
    OSMRoute * sr = SelectedRoute();
    if(sr == nullptr) return nullptr;
    if(_active_point < 0)
    {
        if(sr->closed)
            _active_point = sr->route.count() -1;
        else
            _active_point = -1;
    }

    return ActivePoint();
}

int TrackProvider::ActivePointId()
{
    return _active_point;
}

void TrackProvider::SetActivePoint(int id)
{
    if(_active_point == id)
        return;

    OSMRoute * sr = SelectedRoute();
    if(sr == nullptr)
    {
        _active_point = -1;
        return;
    }

    if(id >= sr->route.count())
    {
        if(sr->closed)
        {
            _active_point = 0;
            return;
        }else
        {
            _active_point = -1;
            return;
        }
    }

    _active_point = id;
}


QList<OSMRoute> TrackProvider::routes = QList<OSMRoute>();
int TrackProvider::_selected_route = -1;
int TrackProvider::_active_point = 0;
