#ifndef MYSQLREPOSITORY_H
#define MYSQLREPOSITORY_H

#include <QJsonObject>
#include <QtSql/QSqlDatabase>

#include "mibUtilsGlobal.h"
#include "mibAbstractRepository.h"

namespace mibot
{

class MIBUTILSSHARED_EXPORT SqlRepository : public AbstractRepository
{
public:
    SqlRepository();
    ~SqlRepository();

    bool Open(QJsonObject &config);
    void Close();


    // AbstractRepository interface
public:
    bool IsOpen();
    GetResult GetResourceByID(QUuid id, AbstractResource *res);
    bool AddNewResource(AbstractResource *res);

private:
    QString asSQLAcceptableValue(QVariant *v, QString field);
    QSqlDatabase _db;
};

}
#endif // MYSQLREPOSITORY_H
