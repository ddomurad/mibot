#ifndef SENSORSREADERFORM_H
#define SENSORSREADERFORM_H

#include <QWidget>

namespace Ui {
class SensorsReaderForm;
}

class SensorsReaderForm : public QWidget
{
    Q_OBJECT

public:
    explicit SensorsReaderForm(QWidget *parent = 0);
    ~SensorsReaderForm();

private:
    Ui::SensorsReaderForm *ui;
};

#endif // SENSORSREADERFORM_H
