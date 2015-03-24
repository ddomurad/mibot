#ifndef CMDSENDER_H
#define CMDSENDER_H

#include <stdio.h>
#include <QCoreApplication>
#include <QtNetwork>
#include <QDebug>

#include <QObject>

#include <mibControllerStatus.h>

class CommandSender : public QObject
{
    Q_OBJECT
public:
    CommandSender(QObject * parent, QLocalSocket * socket):
        QObject(parent)
    {
        _socket = socket;
        t = 0;
    }

    ~CommandSender()
    {}

public slots:
    void sendCommands()
    {
        switch (t)
        {
            case 1: send1();  break;
            case 2: send2();  break;
            case 3: send3();  break;
            case 4: send2();  break;
            default: t = 0; break;
        }

        t++;
    }

    void onRecive()
    {
        QLocalSocket * socket = (QLocalSocket *)sender();
        QByteArray arr = socket->readAll();
        for( int i=0; i<arr.length(); i++)
            printf("[%x]\n", arr.at(i) & 0x000000ff);
    }

private:

    void send1()
    {
        uchar send[] =
        {
            0x08, // write 8 bytes
            0x00, // from addr 0x00
            // data
            0x07, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
        };

        _socket->write((const char*)send, (uint)10);
    }

    void send2()
    {
        uchar send[] =
        {
            0xF0, // write 8 bytes
            0x00 // from addr 0x00
        };

        _socket->write((const char*)send, (uint)2);
    }

    void send3()
    {
        uchar send[] =
        {
            0x01, // write 8 bytes
            0x00, // from addr 0x00
            // data
            0x0B
        };

        _socket->write((const char*)send, (uint)3);
    }

    QLocalSocket * _socket;
    int t;
};
#endif // CMDSENDER_H

