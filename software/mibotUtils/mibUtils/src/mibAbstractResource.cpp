#include "inc/mibAbstractResource.h"
#include "inc/mibLogger.h"

using namespace mibot;

AbstractResource::AbstractResource(QString className):
    _className(className)
{
    AddField("id",QVariant::Uuid);
    _fields["id"]->setValue(QUuid::createUuid());
}

AbstractResource::~AbstractResource()
{
    for( QVariant * v : _fields)
        delete v;
}

QString AbstractResource::ClassName()
{
    return _className;
}

QUuid AbstractResource::Id()
{
    return _fields[ "id" ]->toUuid();
}

QVariant *AbstractResource::Get(QString name)
{
    return _fields[ name ];
}

QStringList AbstractResource::Fields()
{
    return QStringList(_fields.keys());
}

QString AbstractResource::Dump()
{
    QStringList out_str;
    for(QString f : _fields.keys())
    {
        out_str.append( QString("(%1: %2)").arg( f, Get(f)->toString() ) );
    }

    return out_str.join(' ');
}

void AbstractResource::AddField(QString name, QVariant::Type type)
{
    if(_fields.contains(name))
    {
        LOG_ERROR( QString("Can't add a field twice! (%1)").arg(name) );
        return;
    }

    QVariant * v = new QVariant(type);
    _fields.insert( name, v );
}
