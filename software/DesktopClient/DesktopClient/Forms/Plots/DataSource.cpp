#include "DataSource.h"

DataSource::DataSource()
{
    buffer = 0;
    buffer_start=0;
}

DataSource::DataSource(QString name, int size, double min, double max)
{
    buffer = 0;
    buffer_start =0;

    Allocate(size);
    SetLable(name);
    SetMaxValues(max, min);
}

DataSource::~DataSource()
{
    if(buffer != 0)
        delete[] buffer;
}

void DataSource::Allocate(int size)
{
    if(buffer != 0)
        delete[] buffer;

    buffer = new double[size];
    memset(buffer, 0, sizeof(double)*size);
    buffer_size = size;
}

void DataSource::SetMaxValues(double max, double min)
{
    max_y = max;
    min_y = min;
}

void DataSource::SetLable(QString name)
{
    lable = name;
}

void DataSource::PushData(double d)
{
    buffer_start++;
    if(buffer_start >= buffer_size)
        buffer_start = 0;

    buffer[buffer_start] = d;
}

double DataSource::GetValue(int i)
{
    if(i <=  buffer_start)
    {
        return buffer[buffer_start - i];
    }else
    {
        return buffer[buffer_size - (i-buffer_start)];
    }
}
