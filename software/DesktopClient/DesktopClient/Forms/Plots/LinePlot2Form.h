#ifndef LINEPLOT2FORM_H
#define LINEPLOT2FORM_H

#include <QWidget>
#include "DataSource.h"

class LinePlot2Form : public QWidget
{
    Q_OBJECT

public:
    explicit LinePlot2Form(QWidget *parent = 0);
    ~LinePlot2Form();

    void SetDataSource(DataSource * s1, DataSource * s2);
    void SetVisibleArea(double x1, double y1, double x2, double y2);
    void UpdateVisibleAreaFromDataSource();

    DataSource *GetDataSource(int i);

    void SetVectorMode(bool state);
    void SetMarkerMode(bool state);
    void SetAxisVisible(bool state);

    void AddXAxisText(double pos, QString txt);
    void AddYAxisText(double pos, QString txt);

private slots:
    void onTimerUpdate();
    void onContextMenu(QPoint pos);
protected:
    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    DataSource *_s1;
    DataSource *_s2;
    double _border_size;
    double x1,x2,y1,y2;

    bool _draw_markers;
    bool _vectorMode;
    bool _draw_axis;

    QList<QString> _x_axis_values;
    QList<QString> _y_axis_values;

    QList<double> _x_axis_pos;
    QList<double> _y_axis_pos;

    QTimer *_timer;

    double _history_fact;
    bool _draw_zoom_rect;
    QPoint _zoom_rect_start;
    QPoint _zoom_rect_end;
};

#endif // LINEPLOT2FORM_H
