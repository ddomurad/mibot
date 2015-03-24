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

class MIBUTILSSHARED_EXPORT AbstractResourcesSet
{
public:
    AbstractResourcesSet(){}
    virtual ~AbstractResourcesSet() {}

    virtual void InitializeSet(int count) = 0;
    virtual AbstractResource* GetAbstract(int i) = 0;
    virtual AbstractResource* Create() = 0;
};

template <class T>
class MIBUTILSSHARED_EXPORT ResourcesSet : public AbstractResourcesSet
{
public:
    ResourcesSet() {}
    ~ResourcesSet()
    {
        for( T * v : _set)
            delete v;
    }

    void InitializeSet(int count)
    {
        for(T * v : _set)
            delete v;

        _set.clear();

        for(int i=0; i<count; i++)
            _set.append( new T );
    }

    AbstractResource* GetAbstract(int i)
    {
        return _set[i];
    }

    AbstractResource *Create()
    {
        return new T;
    }

    int Count() { return _set.count(); }
    //T * operator[](int i) { return At(i); }
    T * At(int i) { return _set[i]; }

private:
    QList<T*> _set;
};

}
#endif // ABSTRACTRESOURCE_H
