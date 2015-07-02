#ifndef MIBSETTINGSCLIENT_H
#define MIBSETTINGSCLIENT_H

#include <QMutex>
#include <QMutexLocker>
#include "mibSettingsClientAsyncWorker.h"
#include "mibSettingsClientGlobal.h"


namespace mibot
{

#define SC_OP_DEF_TIME 5000

class SettingsClient
{
private:
    SettingsClient();

public:
    static SettingsClient * get();
    ~SettingsClient();

    static bool StartClient(QJsonObject &settings, uint ms = SC_OP_DEF_TIME);
    static void StopClient();
    static QStringList GetResourceList(QString resDir, uint ms = SC_OP_DEF_TIME);
    static void GetResource(QString res);
    static bool SetResource(SettingsObject *obj, uint ms = SC_OP_DEF_TIME);

    template <typename T>
    static T *CreateReource(QString res)
    {
        SettingsClient * client = get();
        QMutexLocker locker(&client->_mutex);

        SettingsObject * cachedObj = client->_asyncWorker->GetCached(res);
        if(cachedObj)
        {
            cachedObj->AddRefCounter();
            return static_cast<T*>(cachedObj);
        }else
        {
            T * newObj = new T(res);
            client->_asyncWorker->AddToCache( static_cast<SettingsObject*>(newObj) );
            return newObj;
        }
    }

    static void ReleaseResource(SettingsObject * obj);

private:
    QMutex _mutex;
    QThread * _asyncWorkerThread;
    SettingsClientAsyncWorker * _asyncWorker;
};

}

#endif // MIBSETTINGSCLIENT_H

