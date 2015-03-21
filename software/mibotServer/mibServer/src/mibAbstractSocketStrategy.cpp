#include "inc/mibAbstractSocketStrategy.h"

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

}

EchoStrategy::~EchoStrategy()
{

}

void EchoStrategy::processNewData(QByteArray d)
{
    qDebug() << d;
}
