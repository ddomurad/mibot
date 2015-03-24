#include <stdio.h>
#include <QCoreApplication>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>

#include <mibControllerStatus.h>
#include "CmdSender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "Server name: ";
    char inp[64];
    scanf("%s", inp);
    QString serverName = QString(inp);

    QTimer * timer = new QTimer(&a);

    QLocalSocket * socket = new QLocalSocket(&a);
    socket->connectToServer(serverName);
    if(!socket->isValid() || !socket->isOpen())
    {
        qDebug() << "error. ...";
        exit(1);
    }

    CommandSender * sender = new CommandSender(&a, socket);

    a.connect( socket, SIGNAL(readyRead()) , sender, SLOT(onRecive()));
    a.connect( timer, SIGNAL(timeout()), sender, SLOT(sendCommands()) );

    timer->start( 110 );

    return a.exec();
}
