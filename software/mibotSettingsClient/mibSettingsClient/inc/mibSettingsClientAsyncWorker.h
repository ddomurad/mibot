#ifndef SETTINGSCLIENTASYNCWORKER_H
#define SETTINGSCLIENTASYNCWORKER_H

#include <QtCore>
#include <QMap>
#include <QObject>
#include <QHostAddress>
#include <QSslCertificate>
#include <QAbstractSocket>
#include <QSslSocket>
#include <QMutex>
#include <QMutexLocker>
#include <mibJSONProtocol.h>

#include "mibClientAsyncWaitEvent.h"
#include "mibSettingsObject.h"

namespace mibot
{

enum class ClientConnectionState {Connected, Disconnected, ConnectionError};

class SettingsClientAsyncWorker : public QObject
{
    Q_OBJECT
public:
    explicit SettingsClientAsyncWorker(QObject *parent = 0);
    ~SettingsClientAsyncWorker();

    bool SetSettings(QJsonObject &settings);
    ClientConnectionState ConnectionState();

    bool WaitForConnection(uint max_ms);
    QStringList GetDirList(QString dirRes, uint ms);
    void GetResource(QString res);

    void AddToCache(SettingsObject *obj);
    void RemoveFromCache(QString res);
    SettingsObject * GetCached(QString res);

    bool UploadObject(SettingsObject *obj, uint ms);
signals:
    void StartClient();
    void StopClient();
    void SendData(QString data);
private slots:
    void onPeerVerifyError(const QSslError &err);
    void onSslErrors(const QList<QSslError> &errors);
    void onSocketError(QAbstractSocket::SocketError);
    void onSocketDisconnected();

    void onSocketData();

    void onStartClinet();
    void onStopClient();

    void onSendData(QString data);
private:
    ClientConnectionState _connectionState;
    QString _serverAddr;
    int    _serverPort;
    QString _serverPeerName;
    QString _cert;
    QString _caCert;
    QSslSocket * _socket;

    JSONProtocol _protocol;
    QMutex       _syncMutex;

    ClientAutoCloseAsyncWaitEvent _connectionEvent;
    ClientAutoCloseAsyncWaitEvent _dirListEvent;
    ClientAutoCloseAsyncWaitEvent _uploadEvent;

    QMap<QString,SettingsObject*> _objectCache;
    QStringList _lastDirList;
    bool _startClient();

    void _processCommands();
    void _processCommand(QJsonObject &cmd);
    void _processNACKResp(QString resource, QString command, QString message);
    void _processCommandListResp(QString, QJsonObject &data);
    void _processCommandGetResp(QString resource, QJsonObject &data);
    void _processCommandSubscribeResp(QString resource);
    void _processCommandUnsubscribeResp(QString resource);
    void _processCommandSetResp(QString resource);
    void _processCommandChange(QString resource, QJsonObject &data);

    void _tryRepairProtocolIfCorupted();

    void _subscribeRes(QString res);
    void _unsubscribeRes(QString res);

    QString createGetDirListCommand(QString res);
    QString createGetCommand(QString res);
    QString createSubscribeCommand(QString res);
    QString createUnsubscribeCommand(QString res);
    QString createSetCommand(QString res, QString data);
};

}
#endif // SETTINGSCLIENTASYNCWORKER_H
