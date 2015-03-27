#include <QDebug>
#include <mibLogger.h>
#include "inc/mibAbstractSocketStrategy.h"
#include "inc/mibDriveStrategy.h"

using namespace mibot;

AbstractSocketStrategy::AbstractSocketStrategy(Connection * connection) :
    QObject(nullptr), _connection(connection)
{}

AbstractSocketStrategy::~AbstractSocketStrategy()
{}

bool AbstractSocketStrategy::ApplyStrategy(Connection * connection)
{
    if(connection->SocketObj() == nullptr) return false;
    QString strategy = connection->SocketObj()->Strategy();

    if(strategy == "echo")
    {
        connection->Strategy = new EchoStrategy(connection);
        return true;
    }
    else if(strategy == "drive")
    {
        connection->Strategy = new DriveStartegy(connection);
        if(!connection->Strategy->init())
        {
            LOG_ERROR("Can't initialize socket stategy.");
            return false;
        }

        return true;
    }

    return false;
}

void AbstractSocketStrategy::ProcessData()
{
    processNewData( ((QTcpSocket*)sender())->readAll() );
}


EchoStrategy::EchoStrategy(Connection * connection)
    :AbstractSocketStrategy(connection)
{
    _cnt ++;
    qDebug() << "EchoStrategy" << _cnt;
}

EchoStrategy::~EchoStrategy()
{
    _cnt --;
    qDebug() << "~EchoStrategy" << _cnt;
}

void EchoStrategy::processNewData(QByteArray d)
{
    _connection->TcpSocket->write(d);
}

bool EchoStrategy::init() { return true; }


int EchoStrategy::_cnt = 0;
