#ifndef PLOTFACTORYFORM_H
#define PLOTFACTORYFORM_H

#include <QWidget>
#include <QMdiArea>
#include "ReferenceCounter.h"

namespace Ui {
class PlotFactoryForm;
}

class PlotFactoryForm : public QWidget, public ReferenceCounter<PlotFactoryForm>
{
    Q_OBJECT

public:
    explicit PlotFactoryForm(QMdiArea * mdiArea, QWidget *parent = 0);
    ~PlotFactoryForm();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::PlotFactoryForm *ui;
    QMdiArea * mdiArea;
};

#endif // PLOTFACTORYFORM_H
