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
    void onUpdateImter();

private:
    OSMWidget * _osm;
    OSMRoute *_route;
    OSMRoute *_active_router;
    QTimer * _update_timer;
};

#endif // GPSPREVIEWFORM_H
