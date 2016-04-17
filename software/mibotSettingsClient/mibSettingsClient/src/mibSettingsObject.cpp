#include <QWaitCondition>
#include <mibLogger.h>
#include "inc/mibSettingsClient.h"
#include "inc/mibSettingsObject.h"

using namespace mibot;

SettingsObject::SettingsObject(QString resource, bool registerForNotification) :
    _resource(resource),
    _synchronized(false),
    _refCounter(1),
    _registerForNotification(registerForNotification),
    _exists(false),
    _sync_sounter(0)
{}

SettingsObject::~SettingsObject()
{
    for(SettingsValue * val : _values)
        delete val;

    _values.clear();
}

QString SettingsObject::Resource()
{
    return _resource;
}

void SettingsObject::AddValue(SettingsValue * v)
{
    _values.append(v);
}

bool SettingsObject::Upload(uint ms)
{
    return SettingsClient::SetResource(this, ms);
}

void SettingsObject::SetSyncFromJson(QJsonObject &obj)
{
    for( SettingsValue * v : _values)
    {
        if(!obj.contains(v->Name()))
        {
            LOG_WARNING(QString("%1 value in object %2 is missing in retrived json data.").arg(v->Name()).arg(Resource()));
        }else
        {
            QJsonValue val = obj.value(v->Name());
            v->SetFromJson( val );
        }
    }

    _exists = true;
    _synchronized = true;
    _sync_sounter++;
    _syncEvent.Open();
}

void SettingsObject::SetSyncNotExists()
{
    _exists = false;
    _synchronized = true;
    _sync_sounter++;
    _syncEvent.Open();
}

void SettingsObject::SetRegistred(bool state)
{
    _isRegistred = state;
}

bool SettingsObject::Sync(uint ms, bool force)
{
    if(force)
        _synchronized = false;
    else if(_synchronized) return true;

    _syncEvent.Close();

    SettingsClient::GetResource( _resource );

    return _syncEvent.Wait(ms) && _exists;
}

bool SettingsObject::IsSynchronized()
{
    return _synchronized;
}

QList<SettingsValue *> SettingsObject::Values()
{
    return _values;
}

uint SettingsObject::GetRefCounter()
{
    return _refCounter;
}

void SettingsObject::AddRefCounter()
{
    _refCounter++;
}

void SettingsObject::RemoverRefCounter()
{
    _refCounter--;
    if(_refCounter < 0)
        _refCounter = 0;
}

bool SettingsObject::SubscribeNotification()
{
    return _registerForNotification;
}

bool SettingsObject::IsSubscribed()
{
    return _isRegistred;
}

QJsonObject SettingsObject::GetAsJson()
{
    QJsonObject obj;
    for(SettingsValue *v : _values)
        obj.insert(v->Name(), v->GetJson());

    return obj;
}

bool SettingsObject::Exists()
{
    return _exists;
}

void SettingsObject::Release()
{
    SettingsClient::ReleaseResource( this );
}

uint SettingsObject::SyncCounter()
{
    return _sync_sounter;
}

QString SettingsObject::Dump()
{
    QString outStr;
    for(SettingsValue* v : _values)
        outStr += QString("[%1] ").arg(v->Dump());

    return outStr;
}

