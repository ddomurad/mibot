#include <QDebug>
#include <mibLogger.h>
#include "inc/mibAbstractSocketStrategy.h"

using namespace mibot;

AbstractSocketStrategy::AbstractSocketStrategy(Connection * connection) :
    QObject(nullptr), _connection(connection)
{}

AbstractSocketStrategy::~AbstractSocketStrategy()
{}

bool AbstractSocketStrategy::ApplyStrategy(Connection * connection)
{
    if(connection->SocketObj == nullptr) return false;
    QString strategy = connection->SocketObj->strategy->value;

    QLibrary plugin( QString("/usr/local/lib/mi_bot/libmib%1Strategy.so").arg(strategy));
    typedef AbstractSocketStrategy * (*StrategyLoader)(Connection *);
    StrategyLoader loader = (StrategyLoader)plugin.resolve("createStrategy");
    if(!loader)
    {
        LOG_ERROR(QString("Can't load strategy '%1' [%2]").arg(strategy,plugin.errorString()));

        return false;
    }else
    {
        connection->Strategy = loader(connection);
        if(!connection->Strategy->init())
        {
            LOG_ERROR( QString("Can't initialize socket stategy '%1'").arg(strategy));
            delete connection->Strategy;
            connection->Strategy = nullptr;
            return false;
        }

        LOG_IMPORTANT( QString("Strategy loaded: '%1'").arg(strategy));
        return true;
    }

    return false;
}

void AbstractSocketStrategy::ProcessData()
{
    processNewData( ((QTcpSocket*)sender())->readAll() );
}
