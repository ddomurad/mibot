#include <mibLogger.h>
#include "inc/mibSettingsClientAsyncWorker.h"

using namespace mibot;

SettingsClientAsyncWorker::SettingsClientAsyncWorker(QObject *parent) : QObject(parent)
{
    _connectionState =  ClientConnectionState::Disconnected;

    connect(this, SIGNAL(StartClient()), this, SLOT(onStartClinet()));
    connect(this, SIGNAL(StopClient()), this, SLOT(onStopClient()));
    connect(this, SIGNAL(SendData(QString)), this, SLOT(onSendData(QString)));
}

SettingsClientAsyncWorker::~SettingsClientAsyncWorker()
{

}

bool SettingsClientAsyncWorker::SetSettings(QJsonObject &settings)
{
    if(!settings["HostAddr"].isString())
    {
        LOG_ERROR("Invalid addr settings");
        return false;
    }

    if(!settings["HostPeerName"].isString())
    {
        LOG_ERROR("Invalid peerName settings");
        return false;
    }

    if(!settings["Cert"].isString())
    {
        LOG_ERROR("Invalid cert settings");
        return false;
    }
    if(!settings["CaCerts"].isString())
    {
        LOG_ERROR("Invalid caCert settings");
        return false;
    }

    _serverAddr = settings["HostAddr"].toString();
    _serverPort = settings["HostPort"].toInt();
    _serverPeerName = settings["HostPeerName"].toString();
    _cert = settings["Cert"].toString();
    _caCert = settings["CaCerts"].toString();

    return true;
}

ClientConnectionState SettingsClientAsyncWorker::ConnectionState()
{
    return _connectionState;
}

bool SettingsClientAsyncWorker::WaitForConnection(uint max_ms)
{
    _connectionEvent.Wait( max_ms );
    return _connectionState == ClientConnectionState::Connected;
}

QStringList SettingsClientAsyncWorker::GetDirList(QString dirRes, uint ms)
{
    _lastDirList.clear();
    QString getDirListCommand = createGetDirListCommand(dirRes);
    emit SendData(getDirListCommand);
    _dirListEvent.Wait(ms);
    return _lastDirList;
}

void SettingsClientAsyncWorker::GetResource(QString res)
{
    QString getCommand = createGetCommand( res );
    emit SendData(getCommand);
}

void SettingsClientAsyncWorker::AddToCache(SettingsObject *obj)
{
    QMutexLocker locket( &_syncMutex );

    if(_objectCache.contains(obj->Resource()))
        return;

    _objectCache.insert(obj->Resource(), obj);

    if(obj->SubscribeNotification())
        _subscribeRes(obj->Resource());
}

void SettingsClientAsyncWorker::RemoveFromCache(QString res)
{
    QMutexLocker locket( &_syncMutex );
    SettingsObject * obj = GetCached( res );
    if(obj != nullptr)
    {
        if(obj->SubscribeNotification())
            _unsubscribeRes(res);

        _objectCache.remove( res );
    }
}

SettingsObject *SettingsClientAsyncWorker::GetCached(QString res)
{
    if(_objectCache.contains(res))
        return _objectCache[res];

    return nullptr;
}

bool SettingsClientAsyncWorker::UploadObject(SettingsObject *obj, uint ms)
{
    QJsonObject jsonObj = obj->GetAsJson();
    QJsonDocument doc(jsonObj);
    QString strData = doc.toJson(QJsonDocument::Compact);

    QString setCommand = createSetCommand(obj->Resource(), strData);
    emit SendData( setCommand );

    return _uploadEvent.Wait(ms);
}

void SettingsClientAsyncWorker::onPeerVerifyError(const QSslError &err)
{
    LOG_ERROR("PeerVerifyError: " + err.errorString());
}

void SettingsClientAsyncWorker::onSslErrors(const QList<QSslError> &errors)
{
    for(QSslError e : errors)
        LOG_ERROR("SslError: " + e.errorString());
}

void SettingsClientAsyncWorker::onSocketError(QAbstractSocket::SocketError)
{
    LOG_ERROR("SocketError: " + _socket->errorString());
}

void SettingsClientAsyncWorker::onSocketDisconnected()
{
    LOG_WARNING("Socket disconnected.");
}

void SettingsClientAsyncWorker::onSocketData()
{
    QByteArray data = _socket->readAll();

    _protocol.PushData( data);
    _processCommands();
}

void SettingsClientAsyncWorker::onStartClinet()
{
    if( _startClient() )
        _connectionState = ClientConnectionState::Connected;
    else
        _connectionState = ClientConnectionState::ConnectionError;

    _connectionEvent.Open();
}

void SettingsClientAsyncWorker::onStopClient()
{
    _socket->disconnectFromHost();
}

void SettingsClientAsyncWorker::onSendData(QString data)
{
    QByteArray byteData;
    byteData.append( data );

    if(_socket->write(byteData) == -1)
    {
        LOG_ERROR("Can't send data.");
        _socket->disconnectFromHost();
    }
}

bool SettingsClientAsyncWorker::_startClient()
{
    QList<QSslCertificate> caCerts =  QSslCertificate::fromPath( _caCert );
    if(caCerts.count() == 0)
    {
        LOG_ERROR("Can't load CaCertificates");
        return false;
    }

    _socket = new QSslSocket(this);
    _socket->setPrivateKey( QString("%1.key").arg(_cert) );
    _socket->setLocalCertificate( QString("%1.crt").arg(_cert) );
    _socket->setCaCertificates( caCerts );
    _socket->setProtocol(QSsl::SecureProtocols);

    connect(_socket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(onPeerVerifyError(QSslError)));
    connect(_socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(onSslErrors(QList<QSslError>)));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    _socket->connectToHostEncrypted(_serverAddr, _serverPort, _serverPeerName);

    if(!_socket->waitForEncrypted( 5000))
    {
        LOG_ERROR( QString("Can't connect to SettingsServer (addr: %1:%2, peerName: %3)")
                   .arg(_serverAddr)
                   .arg(_serverPort)
                   .arg(_serverPeerName));
        return false;
    }

    LOG_INFO( QString("Connected to SettingsServer (addr: %1:%2, peerName: %3)")
              .arg(_serverAddr)
              .arg(_serverPort)
              .arg(_serverPeerName));

    connect(_socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
    connect( _socket, SIGNAL(readyRead()), this, SLOT(onSocketData()));

    return true;
}

void SettingsClientAsyncWorker::_processCommands()
{
    QMutexLocker locket( &_syncMutex );

    _tryRepairProtocolIfCorupted();
    while(_protocol.ContainsValidJSONObject())
    {
        QJsonObject cmd = _protocol.GetPendingObject();
        _processCommand( cmd );

        _tryRepairProtocolIfCorupted();
    }
}

void SettingsClientAsyncWorker::_processCommand(QJsonObject &cmd)
{
    QString resp = cmd["Resp"].toString();
    QString command = cmd["Command"].toString();
    QString resource = cmd["Resource"].toString();
    QJsonObject data =  cmd["Data"].toObject();

    if(resp != "ACK")
    {
        _processNACKResp( resource, command, cmd["Message"].toString());
        return;
    }
    else if(command == "List-Resp")
    {
        _processCommandListResp( resource, data);
        return;
    }else if(command == "Get-Resp")
    {
        _processCommandGetResp(resource, data);
        return;
    }
    else if(command == "Subscribe-Resp")
    {
        _processCommandSubscribeResp(resource);
        return;
    }
    else if(command == "Unsubscribe-Resp")
    {
        _processCommandUnsubscribeResp(resource);
        return;
    }
    else if(command == "Set-Resp")
    {
        _processCommandSetResp(resource);
        return;
    }
    else if(command == "Change")
    {
        _processCommandChange(resource, data);
        return;
    }

    LOG_WARNING(QString("Unknown command: '%1'").arg(command));
}

void SettingsClientAsyncWorker::_processNACKResp(QString resource, QString command, QString message)
{
    LOG_WARNING(QString("NACK %1@'%2' : %3").arg(command,resource, message));

    SettingsObject * obj = GetCached( resource );
    if(obj != nullptr)
        obj->SetSyncNotExists();
}

void SettingsClientAsyncWorker::_processCommandListResp(QString, QJsonObject &data)
{
    QJsonArray resourcesArray = data["Resources"].toArray();

    QStringList resourcesList;

    for(int i=0; i<resourcesArray.count(); i++)
        resourcesList.append(resourcesArray.at(i).toString());

    _lastDirList = resourcesList;
    _dirListEvent.Open();
}

void SettingsClientAsyncWorker::_processCommandGetResp(QString resource, QJsonObject &data)
{
    SettingsObject * cachedObj = GetCached( resource );
    if(cachedObj == nullptr)
    {
        LOG_WARNING("Received object that is not present in chache.");
        return;
    }

    cachedObj->SetSyncFromJson(data);
}

void SettingsClientAsyncWorker::_processCommandSubscribeResp(QString resource)
{
    LOG_INFO(QString("Resource '%1' is subscribed").arg(resource));

    SettingsObject * obj = GetCached(resource);
    if(obj == nullptr)
    {
        LOG_WARNING("Recived subscribtion ack for object that not exists in cache");
        return;
    }

    obj->SetRegistred( true );
}

void SettingsClientAsyncWorker::_processCommandUnsubscribeResp(QString resource)
{
    LOG_INFO(QString("Resource '%1' is unsubscribed").arg(resource));

    SettingsObject * obj = GetCached(resource);
    if(obj != nullptr)
        obj->SetRegistred( false );
}

void SettingsClientAsyncWorker::_processCommandSetResp(QString resource)
{
    LOG_INFO(QString("Resource '%1' uploaded.").arg(resource));

    _uploadEvent.Open();
}

void SettingsClientAsyncWorker::_processCommandChange(QString resource, QJsonObject &data)
{
    SettingsObject * cachedObj = GetCached( resource );
    if(cachedObj == nullptr)
    {
        LOG_WARNING("Received object that is not present in chache.");
        return;
    }

    cachedObj->SetSyncFromJson(data);
}

void SettingsClientAsyncWorker::_tryRepairProtocolIfCorupted()
{
    if(_protocol.IsDataCorupted())
    {
        LOG_WARNING("JsonProtocol has coruped data. Trying to remove invalid data.");
        _protocol.RemoveCoruptedData();
        if(_protocol.IsDataCorupted())
        {
            LOG_ERROR("Can't remoive corupted data from protocol. All protocola data is removed");
            _protocol.RemoveAllData();
        }
    }
}

void SettingsClientAsyncWorker::_subscribeRes(QString res)
{
    QString subscribeCommand = createSubscribeCommand( res );
    emit SendData( subscribeCommand );
}

void SettingsClientAsyncWorker::_unsubscribeRes(QString res)
{
    QString unsubscribeCommand = createUnsubscribeCommand( res );
    emit SendData( unsubscribeCommand );
}


QString SettingsClientAsyncWorker::createGetDirListCommand(QString res)
{
    return QString("{\"Command\":\"List\", \"Resource\":\"%1\"}").arg(res);
}

QString SettingsClientAsyncWorker::createGetCommand(QString res)
{
    return QString("{\"Command\":\"Get\", \"Resource\":\"%1\"}").arg(res);
}

QString SettingsClientAsyncWorker::createSubscribeCommand(QString res)
{
    return QString("{\"Command\":\"Subscribe\", \"Resource\":\"%1\"}").arg(res);
}

QString SettingsClientAsyncWorker::createUnsubscribeCommand(QString res)
{
    return QString("{\"Command\":\"Unsubscribe\", \"Resource\":\"%1\"}").arg(res);
}

QString SettingsClientAsyncWorker::createSetCommand(QString res, QString data)
{
    return QString("{\"Command\":\"Set\", \"Resource\":\"%1\",\"Data\":%2}").arg(res).arg(data);
}

