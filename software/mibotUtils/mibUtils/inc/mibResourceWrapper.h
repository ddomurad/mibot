#ifndef REPOSITORY_H
#define REPOSITORY_H

#include "mibUtilsGlobal.h"
#include "mibAbstractResource.h"
#include "mibAbstractRepository.h"

namespace mibot
{

template <class T>
class MIBUTILSSHARED_EXPORT ResourceWrapper
{
public:
    ResourceWrapper(AbstractRepository *repo) :
        _repo(repo)
    {}

    ~ResourceWrapper()
    {}

    T *getByID(QUuid id)
    {
        T *out = new T;
        if(_repo->GetResourceByID( id, (AbstractResource*)(out) ) != GetResult::Ok)
        {
            delete out;
            return nullptr;
        }

        return out;
    }

    ResourcesSet<T> * getAllByParam(QString param, QVariant * val)
    {
        ResourcesSet<T> * set = new ResourcesSet<T>;

        if( !_repo->GetResourcesByParam( param, val , (AbstractResourcesSet*)(set) ) )
        {
            delete set;
            return nullptr;
        }

        return set;
    }

    ResourcesSet<T> * getAll()
    {
        ResourcesSet<T> * set = new ResourcesSet<T>;

        if( !_repo->GetAllResources(  (AbstractResourcesSet*)(set) ) )
        {
            delete set;
            return nullptr;
        }

        return set;
    }

    bool addNew(T * obj)
    {
        return _repo->AddNewResource( obj );
    }

private:

    AbstractRepository * _repo;
};

}


#endif // REPOSITORY_H
