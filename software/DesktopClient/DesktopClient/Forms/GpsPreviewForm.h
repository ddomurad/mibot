#ifndef GPSPREVIEWFORM_H
#define GPSPREVIEWFORM_H

#include <QWidget>
#include "OSMWidget.h"
#include "ReferenceCounter.h"

class GpsPreviewForm : public QWidget, public ReferenceCounter<GpsPreviewForm>
{
    Q_OBJECT

public:
    explicit GpsPreviewForm(QWidget *parent = 0);
    ~GpsPreviewForm();

    OSMWidget * _osm;
};

#endif // GPSPREVIEWFORM_H
