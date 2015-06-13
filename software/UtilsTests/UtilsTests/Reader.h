#ifndef READER_H
#define READER_H
#include <QCoreApplication>
#include <qobject.h>
#include <QFile>
#include <QDebug>
#include <cstdio>

#include <QtSerialPort/QSerialPort>
#include <mibLogger.h>
#include <mibStandardLoggerBuilder.h>
#include <mibGPSDataParser.h>


class SerialReader : public QObject
{
    Q_OBJECT
public:
    SerialReader(QCoreApplication * app):
        QObject(app)
    {
        port = new QSerialPort(this);
        port->setPortName("/dev/ttyUSB0");
        if(!port->open(QIODevice::ReadOnly))
            exit(1);

        port->setBaudRate(9600);

        connect(port, SIGNAL(readyRead()), this, SLOT(OnDataRead()));

    }

    ~SerialReader()
    {}

public slots:
    void OnDataRead()
    {
        data_buffer += port->readAll();
        if(data_buffer.contains('\n'))
        {
            int index = data_buffer.indexOf('\n');
            QString new_line = data_buffer.left( index -1 );
            data_buffer = data_buffer.right(data_buffer.size() - (index + 1));

            if(possition.CanParse(new_line))
            {
                if(!possition.Parse(new_line))
                    qDebug() << "Error !";
                else
                {
                    if(!possition.IsValid())
                        qDebug() << "Not Valid !";
                    else
                        qDebug() << QString("Position: (%1:%2, %3:%4)")
                                .arg(possition.Position().latitude,10,'g',10)
                                .arg(possition.Position().northSouth)
                                .arg(possition.Position().lognitude,10,'g',10)
                                .arg(possition.Position().eastWast);
                }
            }
        }
    }

private:
    QSerialPort * port;
    QString data_buffer;

    mibot::GPSPositionDataParser possition;

};


#endif // READER_H

