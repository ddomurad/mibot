#ifndef ABSTRACTRESOURCE_H
#define ABSTRACTRESOURCE_H

#include "mibUtilsGlobal.h"
#include "QString"
#include "QMap"
#include "QVariant"

namespace mibot
{

class MIBUTILSSHARED_EXPORT AbstractResource
{
public:
    AbstractResource(QString className);

    virtual ~AbstractResource();

    QString ClassName();
    QUuid Id();

    QVariant *Get(QString name);
    QStringList Fields();

    QString Dump();

protected:
    void AddField(QString name, QVariant::Type type);

private:
    QString                   _className;
    QMap<QString,QVariant*>    _fields;
};

}
#endif // ABSTRACTRESOURCE_H
