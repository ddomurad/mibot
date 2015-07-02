#include "../inc/mibSettingsClient.h"
#include "mibLogger.h"
#include "inc/mibSettingsObject.h"

using namespace mibot;


SettingsClient::SettingsClient()
{
    _asyncWorker = new SettingsClientAsyncWorker(nullptr);
    _asyncWorkerThread = new QThread(nullptr);

    _asyncWorker->moveToThread( _asyncWorkerThread );
    _asyncWorkerThread->start();
}

SettingsClient::~SettingsClient()
{
    delete _asyncWorker;
}

bool SettingsClient::StartClient(QJsonObject &settings, uint ms)
{
    SettingsClient * client = get();
    if(!client->_asyncWorker->SetSettings(settings)) return false;
    emit client->_asyncWorker->StartClient();
    return client->_asyncWorker->WaitForConnection(ms);
}

void SettingsClient::StopClient()
{
    SettingsClient * client = get();
    emit client->_asyncWorker->StopClient();
}

QStringList SettingsClient::GetResourceList(QString resDir, uint ms)
{
    SettingsClient * client = get();
    return client->_asyncWorker->GetDirList( resDir, ms);
}

void SettingsClient::GetResource(QString res)
{
    SettingsClient * client = get();
    client->_asyncWorker->GetResource( res );
}

bool SettingsClient::SetResource(SettingsObject *obj, uint ms)
{
    SettingsClient * client = get();
    return client->_asyncWorker->UploadObject(obj, ms);
}

void SettingsClient::ReleaseResource(SettingsObject *obj)
{
    SettingsClient * client = get();
    QMutexLocker locket(&client->_mutex);
    if(obj->GetRefCounter() == 1)
    {
        client->_asyncWorker->RemoveFromCache( obj->Resource() );
        delete obj;
    }else
    {
        obj->RemoverRefCounter();
    }
}

SettingsClient *SettingsClient::get()
{
    static SettingsClient client;
    return &client;
}
