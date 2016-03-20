
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "JsInput.h"
#include <QDebug>

struct js_event {
    unsigned int time;	/* event timestamp in milliseconds */
    short value;   /* value */
    unsigned char type;     /* event type */
    unsigned char number;   /* axis/button number */
};

JsInput::JsInput(QObject *parent) :
    QThread( parent )
{
    status = false;
    shall_run = false;
    js_dev = 0;
    for(int i=0;i<8;i++) axis[i] = 0;
    for(int i=0;i<16;i++) btn[i] = 0;
}

JsInput::~JsInput()
{}

void JsInput::Start(QString dev)
{
    if(status == true || js_dev != 0)
    {
        qDebug() << "Js thread is already running";
    }

    status = true;

    js_dev = open(dev.toStdString().c_str(), O_RDONLY | O_NONBLOCK);

    if(js_dev < 0)
    {
        qDebug() << "Can't open js input file.";
        status = false;
        return;
    }

    this->start();
}

void JsInput::Stop()
{
    if(status == false)
    {
        qDebug() << "Js input thread is already stoped.";
        return;
    }

    shall_run = false;
    if(!this->wait( 1000 ))
        qDebug() << "Can't stop js input thread.";

}

bool JsInput::Status()
{
    return status;
}

void JsInput::run()
{
    js_event event;
    uint struct_size = sizeof(event);
    shall_run = true;
    while(shall_run)
    {
        QThread::msleep(1);
        int rsize = read( js_dev , (char*)&event, struct_size);

        if(rsize == -1)
        {
            continue;
        }else if ((uint)rsize != struct_size)
        {
            qDebug() << "js read file error ( rsize != struct_size )";
            shall_run = false;
        }else
        {
            if(event.type == 0x02) // axis
            {
                axis[event.number] = event.value;
            }
            else if(event.type == 0x01)
            {
                btn[event.number] = event.value == 1 ? true : false;
            }
        }
    }

    close(js_dev);
    js_dev = 0;

    status = false;
}

