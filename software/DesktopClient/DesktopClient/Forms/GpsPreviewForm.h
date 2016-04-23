#ifndef GPSPREVIEWFORM_H
#define GPSPREVIEWFORM_H

#include <QWidget>
#include "OSMWidget.h"
#include "ReferenceCounter.h"
#include "Clients/RoverClientsProvider.h"

class GpsPreviewForm : public QWidget, public ReferenceCounter<GpsPreviewForm>
{
    Q_OBJECT

public:
    explicit GpsPreviewForm(QWidget *parent = 0);
    ~GpsPreviewForm();

private slots:
    void onSensorData(RoverSensors);

private:
    OSMWidget * _osm;
    OSMRoute *_route;
};

#endif // GPSPREVIEWFORM_H
