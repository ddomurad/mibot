#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QString>

class DataSource
{
public:
    DataSource();
    DataSource(QString name, int size, double min, double max);
    ~DataSource();

    void Allocate(int size);
    void SetMaxValues(double max, double min);
    void SetLable(QString name);

    double max_y;
    double min_y;

    void PushData(double d);

    double GetValue(int i);

    QString lable;

    int buffer_size;
    int buffer_start;

private:
        double * buffer;
};

#endif // DATASOURCE_H
