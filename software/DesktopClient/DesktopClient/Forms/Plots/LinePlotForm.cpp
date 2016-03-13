#include <QPainter>
#include <QPaintEvent>
#include "LinePlotForm.h"

#include <QMenu>

LinePlotForm::LinePlotForm(QWidget *parent) :
    QWidget(parent)
{    
    _data_source = 0;
    _left_border_size = 50;

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
    _timer->start(50);

    _autoScale = false;
    _statusMarkers = false;
    _as_max = 0;
    _as_min = 0;

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)));
}

LinePlotForm::~LinePlotForm()
{}

void LinePlotForm::SetDataSource(DataSource *source)
{
    _data_source = source;
}

DataSource *LinePlotForm::GetDataSource()
{
    return _data_source;
}


void LinePlotForm::AddHLine(QString lable, double x)
{
    _h_labels.append(lable);
    _h_lines.append(x);
}

void LinePlotForm::AutoScale(bool state)
{
    _autoScale = state;
}

void LinePlotForm::StatMarkers(bool state)
{
    _statusMarkers = state;
}

void LinePlotForm::onTimerUpdate()
{
    update();
}

void LinePlotForm::onContextMenu(QPoint pos)
{
    QString ascaleOn = "Auto Y scale ON";
    QString ascaleOff = "Auto Y scale OFF";
    QString statsMarkerOn = "Stats markers ON";
    QString statsMarkerOff = "Stats markers OFF";
    QString addMarker = "Add marker";
    QString clearMarker = "Clear markers";

    QMenu menu;
    if(_autoScale)
        menu.addAction(ascaleOff);
    else
        menu.addAction(ascaleOn);

    if(_statusMarkers)
        menu.addAction(statsMarkerOff);
    else
        menu.addAction(statsMarkerOn);

    menu.addAction(addMarker);
    menu.addAction(clearMarker);

    QPoint gpos = mapToGlobal(pos);
    QAction *selectedAction = menu.exec(gpos);
    if(selectedAction == nullptr)
        return;

    if(selectedAction->text() == ascaleOn)
        AutoScale(true);

    if(selectedAction->text() == ascaleOff)
        AutoScale(false);

    if(selectedAction->text() == addMarker)
    {
        double dy = 0;
        if(_autoScale)
            dy = ((double)this->geometry().height()) / (_as_max - _as_min);
        else
            dy = ((double)this->geometry().height()) / (_data_source->max_y - _data_source->min_y);

        double yValue = (this->geometry().height() - pos.y()) / dy;
        if(_autoScale)
             yValue += _as_min;
        else
            yValue += _data_source->min_y;
        AddHLine(QString::number(yValue), yValue);
    }

    if(selectedAction->text() == statsMarkerOn)
        StatMarkers(true);

    if(selectedAction->text() == statsMarkerOff)
        StatMarkers(false);

    if(selectedAction->text() == clearMarker)
    {
        _h_labels.clear();
        _h_lines.clear();
    }
}

void LinePlotForm::paintEvent(QPaintEvent * pe)
{
    QPainter painter;
    painter.begin(this);

    painter.fillRect(pe->rect(), Qt::black);

    double dx = (double)this->geometry().width() / _data_source->buffer_size;

    double dy = 0;
    double bdn = 0;

    if(_autoScale)
    {
        _as_min -= (_as_max - _as_min)*0.02;
        dy = ((double)this->geometry().height()) / (_as_max - _as_min);
        bdn = (double)this->geometry().height() + _as_min*dy;
    }
    else
    {
        dy = ((double)this->geometry().height()) / (_data_source->max_y - _data_source->min_y);
        bdn = (double)this->geometry().height() + _data_source->min_y*dy;
    }

    painter.setPen(QColor(Qt::green));

    painter.drawLine(_left_border_size,(double)this->geometry().height(),_left_border_size,0);

    for(int i=0;i<_data_source->buffer_size;i++)
    {
        double value = _data_source->GetValue(i);
        painter.drawLine(_left_border_size + dx*i,bdn-value*dy,_left_border_size + dx*(i+1),bdn-_data_source->GetValue(i+1)*dy);

        if(_statusMarkers)
        {
            if(i == 0)
            {
                _sm_max = value;
                _sm_min = value;
            }

            if(value > _sm_max) _sm_max = value;
            if(value < _sm_min) _sm_min = value;
            _sm_avg += value;
        }

        if(_autoScale)
        {
            if(i == 0)
            {
                _as_max = value;
                _as_min = value;
            }

            if(value > _as_max) _as_max = value;
            if(value < _as_min) _as_min = value;
        }
    }

    if(_statusMarkers)
        _sm_avg /= _data_source->buffer_size;

    painter.setPen(QColor(Qt::yellow));
    for(int i=0;i<_h_lines.count();i++)
    {
        double y = _h_lines[i];
        painter.drawText(0,bdn-y*dy,_left_border_size,10,Qt::AlignRight | Qt::AlignVCenter,_h_labels[i]);
        painter.drawLine(_left_border_size, bdn-y*dy, this->geometry().width(), bdn-y*dy );
    }

    if(_statusMarkers)
    {
        painter.setPen(QColor(Qt::red));
        _drawHLine(&painter, _sm_max, bdn, dy, QString::number(_sm_max));
        _drawHLine(&painter, _sm_min, bdn, dy, QString::number(_sm_min));
        _drawHLine(&painter, _sm_avg, bdn, dy, QString::number(_sm_avg));

    }

    painter.setPen(QColor(Qt::green));
    painter.rotate(-90);
    painter.drawText(-this->geometry().height(),0,this->geometry().height(),_left_border_size,Qt::AlignCenter, _data_source->lable);
    painter.end();
}

void LinePlotForm::_drawHLine(QPainter * painter, double y, double bdn, double dy, QString txt)
{
    painter->drawText(0,bdn-y*dy,_left_border_size,10,Qt::AlignRight | Qt::AlignVCenter, txt);
    painter->drawLine(_left_border_size, bdn-y*dy, this->geometry().width(), bdn-y*dy );
}
