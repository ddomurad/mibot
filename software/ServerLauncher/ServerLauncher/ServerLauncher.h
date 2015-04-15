#ifndef SERVERLAUNCHER_H
#define SERVERLAUNCHER_H

#include <QObject>
#include <QCoreApplication>
#include <mibServer.h>

class ServerLauncher : public QObject
{
    Q_OBJECT
public:
    explicit ServerLauncher(QCoreApplication *parent = 0);
    ~ServerLauncher();

    bool Init(QStringList args);
    void Close();

signals:
    void Closed();

private slots:
    void onServerStarted();
    void onServerStoped();

private:
    mibot::Server * _server;
    void signalHandler(int sig);
    static void _signalHandler(int sig);
    static ServerLauncher *_launcher;
};

#endif // SERVERLAUNCHER_H
