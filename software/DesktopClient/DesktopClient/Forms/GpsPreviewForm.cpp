#include "GpsPreviewForm.h"
#include <QGridLayout>
#include "Settings/AppSettings.h"

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
}

GpsPreviewForm::~GpsPreviewForm()
{}
