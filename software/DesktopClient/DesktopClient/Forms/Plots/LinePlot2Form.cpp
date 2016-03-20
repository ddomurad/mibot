#include "LinePlot2Form.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTimer>
#include <QMenu>

LinePlot2Form::LinePlot2Form(QWidget *parent) :
    QWidget(parent)
{
    _border_size = 25;

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
    _timer->start(50);

    _draw_markers = false;
    _history_fact = 1.0f;
    _draw_zoom_rect = false;
    _draw_axis = false;

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(onContextMenu(QPoint)));
}

LinePlot2Form::~LinePlot2Form()
{
}

void LinePlot2Form::SetDataSource(DataSource *s1, DataSource *s2)
{
    _s1 = s1;
    _s2 = s2;
}

void LinePlot2Form::UpdateVisibleAreaFromDataSource()
{
    x1 = _s1->min_y;
    x2 = _s1->max_y;
    y1 = _s2->min_y;
    y2 = _s2->max_y;
}

void LinePlot2Form::SetVisibleArea(double x1, double y1, double x2, double y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
}

DataSource *LinePlot2Form::GetDataSource(int i)
{
    if(i == 0)
        return _s1;

    if(i == 1)
        return _s2;

    return 0;
}

void LinePlot2Form::SetVectorMode(bool state)
{
    _vectorMode = state;
}

void LinePlot2Form::SetMarkerMode(bool state)
{
    _draw_markers = state;
}

void LinePlot2Form::SetAxisVisible(bool state)
{
    _draw_axis = state;
}

void LinePlot2Form::AddXAxisText(double pos, QString txt)
{
    _x_axis_pos.append(pos);
    _x_axis_values.append(txt);
}

void LinePlot2Form::AddYAxisText(double pos, QString txt)
{
    _y_axis_pos.append(pos);
    _y_axis_values.append(txt);
}

void LinePlot2Form::onTimerUpdate()
{
    update();
}

void LinePlot2Form::onContextMenu(QPoint pos)
{
    QString toogleAxis = "Toogle axis";
    QString toogleMarkers = "Toogle markers";
    QString toogleVector = "Toogle vector";

    QString addXMarker = "Add H marker";
    QString addYMarker = "Add V marker";
    QString addXYMarker = "Add V-H marker";
    QString clearMarker = "Clear markers";

    QString setHist1 = "History: 1";
    QString setHist05 = "History: 0.5";
    QString setHist025 = "History: 0.25";
    QString setHist01 = "History: 0.1";
    QString setHist001 = "History: 0.01";

    QString resetZoom = "Reset zoom";


    QMenu menu;
    menu.addAction(toogleAxis);
    menu.addAction(toogleMarkers);
    menu.addAction(toogleVector);
    menu.addSeparator();
    menu.addAction(addXMarker);
    menu.addAction(addYMarker);
    menu.addAction(addXYMarker);
    menu.addAction(clearMarker);
    menu.addSeparator();
    menu.addAction(setHist1);
    menu.addAction(setHist05 );
    menu.addAction(setHist025);
    menu.addAction(setHist01 );
    menu.addAction(setHist001);
    menu.addSeparator();
    menu.addAction(resetZoom );

    QPoint gpos = mapToGlobal(pos);
    QAction *selectedAction = menu.exec(gpos);

    if(selectedAction == nullptr)
        return;

    if(selectedAction->text() == toogleAxis)
    {
        SetAxisVisible( !_draw_axis );
    }

    if(selectedAction->text() == addXMarker || selectedAction->text() == addXYMarker)
    {
        double dy = ((double)this->geometry().height() - _border_size*2) / (_s2->max_y - _s2->min_y);

        double yValue = (this->geometry().height() - (pos.y() +_border_size)) / dy;
        yValue += _s2->min_y;

        AddYAxisText(yValue, QString::number(yValue));
    }

    if(selectedAction->text() == addYMarker || selectedAction->text() == addXYMarker)
    {
        double dy = ((double)this->geometry().width() - _border_size*2) / (_s1->max_y - _s1->min_y);

        double yValue = (this->geometry().width() - (pos.x() + _border_size)) / dy;
        yValue += _s1->min_y;

        AddXAxisText(-yValue, QString::number(-yValue));
    }

    if(selectedAction->text() == toogleMarkers)
        SetMarkerMode(!_draw_markers);
    else if(selectedAction->text() == toogleVector)
        SetVectorMode(!_vectorMode);
    else if(selectedAction->text() == clearMarker)
    {
        _x_axis_pos.clear();
        _x_axis_values.clear();
        _y_axis_pos.clear();
        _y_axis_values.clear();
    }
    else if(selectedAction->text() == setHist1)
        _history_fact = 1.0;
    else if(selectedAction->text() == setHist05)
        _history_fact = 0.5;
    else if(selectedAction->text() == setHist025)
        _history_fact = 0.25;
    else if(selectedAction->text() == setHist01)
        _history_fact = 0.1;
    else if(selectedAction->text() == setHist001)
        _history_fact = 0.01;
    else if(selectedAction->text() == resetZoom)
        UpdateVisibleAreaFromDataSource();
}

void LinePlot2Form::paintEvent(QPaintEvent * pe)
{
    QPainter painter;
    painter.begin(this);
    painter.fillRect(pe->rect(), Qt::black);


    if(_s1->buffer_size != _s2->buffer_size)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawText(pe->rect(), Qt::AlignCenter | Qt::AlignVCenter, "_s1->buffer_size != _s2->buffer_size");
        return;
    }

    QPen pen = QPen(Qt::red);

    int bs = _s1->buffer_size * _history_fact;
    double dx = ((double)this->rect().width() - _border_size*2)/ (x2-x1);
    double dy = ((double)this->rect().height() - _border_size*2)/ (y2-y1);

    double zx = (x2-x1)/2;
    double zy = (y2-y1)/2;

    if(_draw_axis)
    {
        double x0 = _border_size + zx*dx;
        double y0 = _border_size + zy*dy;

        painter.setPen(QPen(QColor(0,0,100)));
        painter.drawLine(_border_size + (x1 +zx)*dx,y0,_border_size + (x2 +zx)*dx,y0);
        painter.drawLine(x0, _border_size + (y1 +zy)*dy,x0,_border_size + (y2 +zy)*dy);

        painter.setPen(QPen(QColor(100,100,0)));
        for(int i=0;i<_x_axis_values.count(); i++)
        {
            painter.drawPoint(_border_size + (_x_axis_pos[i] + zx)*dx,y0);
            painter.drawText(_border_size + (_x_axis_pos[i] + zx)*dx,y0,25,100,Qt::AlignLeft, _x_axis_values[i]);
            if(_draw_markers)
            {
                painter.drawLine(_border_size + (_x_axis_pos[i] + zx)*dx,
                                 _border_size,
                                 _border_size + (_x_axis_pos[i] + zx)*dx,
                                 this->geometry().height() - _border_size);
            }
        }

        for(int i=0;i<_y_axis_values.count(); i++)
        {
            painter.drawPoint(x0, _border_size + (zy - _y_axis_pos[i])*dy);
            painter.drawText(x0, _border_size + (zy - _y_axis_pos[i])*dy,25,100,Qt::AlignLeft, _y_axis_values[i]);
            if(_draw_markers)
            {
                painter.drawLine(_border_size ,
                                 _border_size + (zy - _y_axis_pos[i])*dy,
                                 this->geometry().width() - _border_size,
                                 _border_size + (zy - _y_axis_pos[i])*dy);
            }
        }
    }

    for(int i=bs-2;i>=0;i--)
    {
        pen.setColor(QColor(0,255.0 * ((float)(bs - i)/(float)bs),0));
        painter.setPen(pen);

        double x = _border_size + (_s1->GetValue(i) +zx )* dx;
        double y = _border_size + (zy - _s2->GetValue(i))* dy;

        double lx = _border_size + (_s1->GetValue(i+1) + zx)* dx;
        double ly = _border_size + (zy - _s2->GetValue(i+1))* dy;

        painter.drawLine(lx,ly,x,y);
    }

    if(_vectorMode)
    {
        double x1 = _border_size + zx*dx;
        double y1 = _border_size + zy*dy;

        double x2 = _border_size + (_s1->GetValue(0) +zx )* dx;
        double y2 = _border_size + (zy - _s2->GetValue(0) )* dy;

        painter.setPen(QPen(Qt::red));
        painter.drawLine(x1,y1,x2,y2);
    }

    if(_draw_zoom_rect)
    {
        painter.setPen(QPen(Qt::red));
        painter.drawRect(QRect(_zoom_rect_start, _zoom_rect_end));
    }

    painter.setPen(QPen(Qt::yellow));
    painter.drawRect(_border_size,_border_size, this->rect().width() - _border_size*2,this->rect().height() - _border_size*2);
    painter.drawText(0, this->rect().height() - _border_size,this->rect().width(), _border_size, Qt::AlignCenter, _s1->lable);

    painter.rotate(-90);
    painter.drawText(-this->geometry().height(),0,this->geometry().height(),_border_size,Qt::AlignCenter, _s2->lable);

    painter.end();
}

void LinePlot2Form::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        _zoom_rect_start = e->pos();
        _zoom_rect_end = e->pos();
        _draw_zoom_rect = true;
    }
}

void LinePlot2Form::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        _draw_zoom_rect = false;

        double _x1 = _zoom_rect_start.x();
        double _y1 = _zoom_rect_start.y();
        double _x2 = _zoom_rect_end.x();
        double _y2 = _zoom_rect_end.y();

        if(_x1 > _x2) { _x1 = _x1 + _x2; _x2 = _x1 - _x2; _x1 = _x1 - _x2; }
        if(_y1 > _y2) { _y1 = _y1 + _y2; _y2 = _y1 - _y2; _y1 = _y1 - _y2; }

        double dx = ((double)this->rect().width() - _border_size*2)/ (x2-x1);
        double dy = ((double)this->rect().height() - _border_size*2)/ (y2-y1);
        double zx = (x2-x1)/2;
        double zy = (y2-y1)/2;

        _x1 = (_x1/dx) - zx;
        _x2 = (_x2/dx) - zx;
        _y1 = (_y1/dy) - zy;
        _y2 = (_y2/dy) - zy;


        x1 = _border_size + _x1;
        x2 = _border_size + _x2;
        y1 = _border_size + _y1;
        y2 = _border_size + _y2;
    }
}

void LinePlot2Form::mouseMoveEvent(QMouseEvent *e)
{
    if(_draw_zoom_rect)
        _zoom_rect_end = e->pos();
}
