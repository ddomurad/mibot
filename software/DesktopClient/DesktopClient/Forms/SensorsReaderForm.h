#ifndef SENSORSREADERFORM_H
#define SENSORSREADERFORM_H

#include <QWidget>
#include "ReferenceCounter.h"
#include "./Clients/RoverSensorClient.h"

namespace Ui {
class SensorsReaderForm;
}

class SensorsReaderForm : public QWidget, public ReferenceCounter<SensorsReaderForm>
{
    Q_OBJECT

public:
    explicit SensorsReaderForm(QWidget *parent = 0);
    ~SensorsReaderForm();
public slots:
    void onSensorData(RoverSensors sensors);

private:
    Ui::SensorsReaderForm *ui;

};

#endif // SENSORSREADERFORM_H
