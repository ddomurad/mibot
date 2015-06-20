#ifndef ROUTEEDITORFORM_H
#define ROUTEEDITORFORM_H

#include <QWidget>

namespace Ui {
class RouteEditorForm;
}

class RouteEditorForm : public QWidget
{
    Q_OBJECT

public:
    explicit RouteEditorForm(QWidget *parent = 0);
    ~RouteEditorForm();

private:
    Ui::RouteEditorForm *ui;
};

#endif // ROUTEEDITORFORM_H
