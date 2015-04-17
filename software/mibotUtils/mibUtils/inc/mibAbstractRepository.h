#ifndef ABSTRACTREPOSITORY_H
#define ABSTRACTREPOSITORY_H

#include "mibAbstractResource.h"
#include <QUuid>

namespace mibot
{

enum class GetResult
{
    Ok,
    NoResource,
    Error
};

class AbstractRepository
{
public:
    AbstractRepository();
    virtual ~AbstractRepository();

    virtual bool IsOpen() = 0;
    virtual GetResult GetResourceByID( QUuid id ,AbstractResource *) = 0;
    virtual bool      AddNewResource( AbstractResource * ) = 0;
    virtual bool      GetResourcesByParam( QString , QVariant* , AbstractResourcesSet* ) = 0;
    virtual bool      GetAllResources( AbstractResourcesSet* ) = 0;
};

}
#endif // ABSTRACTREPOSITORY_H
