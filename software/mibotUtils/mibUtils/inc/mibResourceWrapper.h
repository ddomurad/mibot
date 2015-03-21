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

    bool addNew(T * obj)
    {
        return _repo->AddNewResource( obj );
    }

private:

    AbstractRepository * _repo;
};

}


#endif // REPOSITORY_H
