#ifndef SETTINGSOBJECT_H
#define SETTINGSOBJECT_H

#include <QtCore>
#include <QMutex>
#include <mibAsyncWaitEvent.h>

#include "mibSettingsValue.h"

namespace mibot
{

#define SO_DEF_SYNC_TIME 5000
class SettingsObject
{
public:
    SettingsObject(QString resource, bool registerForNotification);
    ~SettingsObject();

    QString Resource();
    void AddValue(SettingsValue*);
    bool Upload(uint ms);

    void SetSyncFromJson(QJsonObject &obj);
    void SetSyncNotExists();
    void SetRegistred(bool state);

    bool Sync(uint ms = SO_DEF_SYNC_TIME, bool force = false);

    bool IsSynchronized();
    QList<SettingsValue*> Values();

    uint GetRefCounter();
    void AddRefCounter();
    void RemoverRefCounter();
    bool SubscribeNotification();
    bool IsSubscribed();

    QJsonObject GetAsJson();
    bool Exists();
    void Release();

    template <typename T>
    T * CloneRef()
    {
        AddRefCounter();
        return static_cast<T*>(this);
    }

    QString Dump();
protected:
    QList<SettingsValue*> _values;
private:
    QString _resource;

    AutoOpenAsyncWaitEvent _syncEvent;
    bool _synchronized;
    int _refCounter;
    bool _registerForNotification;
    bool _isRegistred;

    bool _exists;
};

}
#endif // SETTINGSOBJECT_H
