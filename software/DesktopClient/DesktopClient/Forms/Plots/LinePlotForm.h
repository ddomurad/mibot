#ifndef LINEPLOTFORM_H
#define LINEPLOTFORM_H

#include"DataSource.h"
#include <QWidget>
#include <QTimer>

namespace Ui {
class LinePlotForm;
}

class LinePlotForm : public QWidget
{
    Q_OBJECT

public:
    explicit LinePlotForm(QWidget *parent = 0);
    ~LinePlotForm();

    void SetDataSource(DataSource * source);
    DataSource * GetDataSource();
    void AddHLine(QString lable, double y);

    void AutoScale(bool state);
    void StatMarkers(bool state);
private slots:
    void onTimerUpdate();
    void onContextMenu(QPoint);
protected:
    void paintEvent(QPaintEvent *);

private:
    QList<double> _h_lines;
    QList<QString> _h_labels;
    DataSource  *_data_source;

    int _left_border_size;

    QTimer * _timer;

    bool _autoScale;
    double _as_max;
    double _as_min;

    bool _statusMarkers;
    double _sm_min;
    double _sm_max;
    double _sm_avg;

    void _drawHLine(QPainter *painter, double y, double bdn, double dy, QString txt);
};

#endif // LINEPLOTFORM_H
