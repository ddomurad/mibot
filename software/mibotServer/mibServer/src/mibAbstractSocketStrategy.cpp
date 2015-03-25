#include <mibLogger.h>
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

    if(strategy == "echo")
    {
        connection->Strategy = new EchoStrategy(connection);
        return true;
    }
    else if(strategy == "engines_controller")
    {
        connection->Strategy = new EnginesDriverStartegy(connection);
        if(!connection->Strategy->init())
        {
            DEFLOG_ERROR("Can't initialize socket stategy.");
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


EnginesDriverStartegy::EnginesDriverStartegy(Connection *connection)
    : AbstractSocketStrategy(connection)
{
    _cnt++;
    qDebug() << "EnginesDriverStartegy" << _cnt;

    socket = new QLocalSocket(this);
    connect( socket, SIGNAL(readyRead()), this, SLOT(onLocalSocketRead()) );
}

EnginesDriverStartegy::~EnginesDriverStartegy()
{
    _cnt--;
    qDebug() << "~EnginesDriverStartegy" << _cnt;

    if(socket != nullptr) socket->close();
}

void EnginesDriverStartegy::onLocalSocketRead()
{
    _connection->TcpSocket->write( socket->readAll() );
}

void EnginesDriverStartegy::processNewData(QByteArray b)
{
    if(socket->state() != QLocalSocket::ConnectedState) return;
    socket->write( b );
}

bool EnginesDriverStartegy::init()
{
    if(socket == nullptr)
    {
        DEFLOG_ERROR("Socket is not created.");
        return false;
    }

    QString param_name = "local_server_name";
    ResourcesSet<GlobalConfigRes> * set = GlobalAccess::GlobalConfigsByKey(param_name);
    if(set == nullptr)
    {
        DEFLOG_ERROR("Can't read global configuration.");
        return false;
    }

    if(set->Count() == 0)
    {
        DEFLOG_ERROR("Can't find global configuration: '" + param_name + "'");
        delete set;
        return false;
    }

    if(set->Count() != 1)
    {
        DEFLOG_ERROR("Readed more than 1 global configuration: '" + param_name + "'");
        delete set;
        return false;
    }

    QString local_server_name = set->At(0)->Value();
    delete set;

    socket->connectToServer( local_server_name );
    if(!socket->waitForConnected( 1000 ))
    {
        DEFLOG_ERROR("Can't connect to local server: '" + local_server_name + "'");
        return false;
    }

    DEFLOG_INFO("EnginesDriver strategy is ready to use.");

    return true;
}

int EnginesDriverStartegy::_cnt = 0;

