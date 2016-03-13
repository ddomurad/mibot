#ifndef INFOSERVICEFORM_H
#define INFOSERVICEFORM_H

#include <QWidget>
#include "./Clients/Socket.h"
#include "./Clients/mibJSONProtocol.h"
#include "./Clients/InfoServiceClient.h"

namespace Ui {
class InfoServiceForm;
}

class InfoServiceForm : public QWidget
{
    Q_OBJECT

public:
    explicit InfoServiceForm(QWidget *parent = 0);
    ~InfoServiceForm();

private slots:
    void on_pushButton_refresh_clicked();
    void onRefreshDone();

private:
    Ui::InfoServiceForm *ui;
    InfoServiceClient * _infoClient;
};

#endif // INFOSERVICEFORM_H
