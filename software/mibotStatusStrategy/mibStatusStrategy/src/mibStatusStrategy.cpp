#include "inc/mibStatusStrategy.h"

using namespace mibot;

StatusStrategy::StatusStrategy(Connection *connection) :
    AbstractSocketStrategy(connection)
{}

StatusStrategy::~StatusStrategy()
{}

void StatusStrategy::processNewData(QByteArray)
{
}

bool StatusStrategy::init()
{
    return true;
}


mibot::AbstractSocketStrategy *createStrategy(mibot::Connection *connection)
{
    return new StatusStrategy(connection);
}
