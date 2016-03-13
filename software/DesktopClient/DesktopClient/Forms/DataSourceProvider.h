#ifndef DATASOURCEPROVIDER_H
#define DATASOURCEPROVIDER_H

#include <QtCore>
#include "./Plots/DataSource.h"
#include "./Clients/RoverSensorClient.h"

class DataSourceProvider : QObject
{
    Q_OBJECT
public:
    ~DataSourceProvider();

    static DataSourceProvider * get();
    QList<QString> DataSources();
    DataSource * GetSource(QString name);

public slots:
    void onNewSensorData(RoverSensors readings);
    void fakeTimer();
private:
    DataSourceProvider();
    QList<DataSource*> dataSources;
    QTimer * _timer;
};

#endif // DATASOURCEPROVIDER_H
