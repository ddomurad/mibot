#include "GpsPreviewForm.h"
#include <QGridLayout>
#include "Settings/AppSettings.h"
#include "Other/TrackProvider.h"

GpsPreviewForm::GpsPreviewForm(QWidget *parent) :
    QWidget(parent)
{

    QGridLayout * layout = new QGridLayout(this);
    _osm = new OSMWidget(this);
    layout->addWidget(_osm);
    _osm->show();

    _osm->setResources(
                AppSettings::GetKey( AppSettings::OSM_Server_Address ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Cache_Dir ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Tiles_Format ).toString(),
                AppSettings::GetKey( AppSettings::OSM_Missing_Tile ).toString() );

    _osm->setZoomRange(5,18);
    _osm->setZoom(15);
    _osm->centreGPSPossition(QPointF(19.9229, 50.0767));

    this->setLayout(layout);

    auto sensorClient = RoverClientsProvider::GetRoverSensorClient();
    connect(sensorClient, SIGNAL(newData(RoverSensors)), this, SLOT(onSensorData(RoverSensors)));

    OSMRoute route;
    route.name = "gps_track";
    _osm->addRoute(route);
    _route = _osm->getRoute("gps_track");
    _route->addPoint(QPointF(19.9229, 50.0767));
    _route->getMarker(0)->type = OSMMarkerType::Triangle3;
    _route->editable = false;
    _route->stdFirstMarkerType = OSMMarkerType::Triangle3;

    _update_timer = new QTimer(this);
    connect(_update_timer, SIGNAL(timeout()), this, SLOT(onUpdateImter()));
    _update_timer->start(1000);
}

GpsPreviewForm::~GpsPreviewForm()
{}

void GpsPreviewForm::onSensorData(RoverSensors sensor)
{
    QPointF pos = QPointF(sensor.gpsSensors.lognitude, sensor.gpsSensors.latitude);

    _route->getMarker(0)->possition = pos;
    _route->getMarker(0)->rotate = sensor.gpsSensors.course;
    _osm->centreGPSPossition(pos);
}

void GpsPreviewForm::onUpdateImter()
{
    OSMRoute * route = TrackProvider::SelectedRoute();
    QList<OSMRoute> * routes = _osm->getRoutes();

    while(routes->count() > 1)
        routes->removeAt(1);

    if(route != nullptr)
    {
        route->editable = false;
        routes->push_back(*route);
        if(TrackProvider::ActivePoint() != nullptr)
        {
            int id = TrackProvider::ActivePointId();

            _osm->setTargetMarker(&((*routes)[1]).route[id]);
        }
    }
}
