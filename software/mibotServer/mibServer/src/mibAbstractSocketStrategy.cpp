#include "inc/mibAbstractSocketStrategy.h"
#include <QDebug>

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

    if(strategy != "echo") return false;

    connection->Strategy = new EchoStrategy(connection);

    return true;
}

void AbstractSocketStrategy::ProcessData()
{
    processNewData(((QTcpSocket*)sender())->readAll());
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


int EchoStrategy::_cnt = 0;
