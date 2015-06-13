#ifndef WMSCLIENT_H
#define WMSCLIENT_H

#include <QObject>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class WMSClient : public QObject
{
    Q_OBJECT
public:
    explicit WMSClient(QCoreApplication * app);
    ~WMSClient();

    void Test();

signals:

private slots:

    void slotError(QNetworkReply::NetworkError);
    void slotSslErrors(QList<QSslError>);
    void replyFinished(QNetworkReply*);
    void replyFinished2();
private:
    QNetworkAccessManager * manager;
};

#endif // WMSCLIENT_H
